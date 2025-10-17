#include "hello_world.h"

#include <Arduino.h>
#include <pico/bootrom.h>

#include "app_config.h"
#include "qr_benchmark.h"
#include "serial_ui.h"
#include "status_led.h"

namespace {
struct ButtonState {
  int stableState = HIGH;
  int lastRead = HIGH;
  unsigned long lastChangeMs = 0;
};

unsigned long lastHelloMs = 0;
unsigned long lastHeartbeatMs = 0;
bool heartbeatLedOn = false;
uint32_t helloCount = 0;
ButtonState buttonState;
constexpr char kBootselCommand[] = "::BOOTSEL::";
size_t bootselMatchIndex = 0;

void updateHeartbeat(const unsigned long now) {
  if (now - lastHeartbeatMs < AppConfig::heartbeatIntervalMs) {
    return;
  }

  lastHeartbeatMs = now;
  heartbeatLedOn = !heartbeatLedOn;
  showStatusPixel(heartbeatLedOn ? AppConfig::heartbeatOnColor : AppConfig::heartbeatOffColor);
}

void updateHelloAnnouncer(const unsigned long now) {
  if (now - lastHelloMs < AppConfig::helloIntervalMs) {
    return;
  }

  lastHelloMs = now;
  ++helloCount;
  Serial.print(AppConfig::helloMessage);
  Serial.print(" (count: ");
  Serial.print(helloCount);
  Serial.println(')');
  showStatusPixel(AppConfig::helloColor);
}

void updateButtonMonitor(const unsigned long now) {
  const int reading = digitalRead(AppConfig::buttonPin);
  if (reading != buttonState.lastRead) {
    buttonState.lastRead = reading;
    buttonState.lastChangeMs = now;
  }

  if ((now - buttonState.lastChangeMs) < AppConfig::buttonDebounceMs) {
    return;
  }

  if (reading == buttonState.stableState) {
    return;
  }

  buttonState.stableState = reading;
  if (buttonState.stableState == LOW) {
    Serial.println("BOOT button pressed (GP0 low)");
  } else {
    Serial.println("BOOT button released (GP0 high)");
  }
}

void waitForSerial() {
  static const CRGB waitingPattern[] = {CRGB::Blue, CRGB::Purple, CRGB::Black};
  const size_t patternLength = sizeof(waitingPattern) / sizeof(waitingPattern[0]);
  size_t colorIndex = 0;
  unsigned long lastPatternUpdate = 0;
  const unsigned long start = millis();
  while (!Serial && (millis() - start) < AppConfig::serialWaitTimeoutMs) {
    const unsigned long now = millis();
    if (now - lastPatternUpdate >= 150) {
      lastPatternUpdate = now;
      showStatusPixel(waitingPattern[colorIndex]);
      colorIndex = (colorIndex + 1) % patternLength;
    }
    delay(10);
  }

  if (Serial) {
    showStatusPixel(AppConfig::helloColor);
  } else {
    showStatusPixel(AppConfig::heartbeatOffColor);
  }
}

void checkForBootselTrigger() {
  while (Serial.available() > 0) {
    const char c = Serial.read();
    if (c == kBootselCommand[bootselMatchIndex]) {
      ++bootselMatchIndex;
      if (kBootselCommand[bootselMatchIndex] == '\0') {
        Serial.println("\nEntering BOOTSEL mode (auto-upload request)...");
        Serial.flush();
        delay(20);
        bootselMatchIndex = 0;
        reset_usb_boot(0, 0);
      }
    } else {
      bootselMatchIndex = (c == kBootselCommand[0]) ? 1 : 0;
    }
  }
}
}  // namespace

void setupHelloWorld() {
  initStatusPixel();

  pinMode(AppConfig::buttonPin, INPUT_PULLUP);
  buttonState.lastRead = digitalRead(AppConfig::buttonPin);
  buttonState.stableState = buttonState.lastRead;

  Serial.begin(AppConfig::baudRate);

  waitForSerial();
  printStartupBanner();
  runQrBenchmark();

  Serial.println("\nBenchmark complete. Starting normal operation...");
  Serial.println("---------------------------------------------\n");
  showStatusPixel(AppConfig::benchmarkCompleteColor);
  delay(1000);
}

void loopHelloWorld() {
  const unsigned long now = millis();
  updateHeartbeat(now);
  updateHelloAnnouncer(now);
  updateButtonMonitor(now);
  checkForBootselTrigger();
}
