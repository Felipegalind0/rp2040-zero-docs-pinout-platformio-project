#include <Arduino.h>
#include <FastLED.h>

constexpr unsigned long BAUD_RATE = 115200;
constexpr unsigned long HELLO_INTERVAL_MS = 1000;
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 500;
constexpr unsigned long SERIAL_WAIT_TIMEOUT_MS = 5000;

constexpr uint8_t NEOPIXEL_PIN = 16;
constexpr uint8_t NEOPIXEL_COUNT = 1;
constexpr uint8_t NEOPIXEL_BRIGHTNESS = 128;

CRGB statusPixel[NEOPIXEL_COUNT];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(BAUD_RATE);

  FastLED.addLeds<WS2812B, NEOPIXEL_PIN, GRB>(statusPixel, NEOPIXEL_COUNT);
  FastLED.setBrightness(NEOPIXEL_BRIGHTNESS);
  fill_solid(statusPixel, NEOPIXEL_COUNT, CRGB::White);
  FastLED.show();

  const unsigned long start = millis();
  while (!Serial && (millis() - start) < SERIAL_WAIT_TIMEOUT_MS) {
    delay(10);
  }

  Serial.println();
  Serial.println("Raspberry Pi Pico says hello!");
  Serial.println("USB serial link is ready.");
  Serial.println("Project repo: https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project");
  Serial.println();
}

void loop() {
  static unsigned long lastHello = 0;
  static unsigned long lastHeartbeat = 0;
  static bool ledOn = false;
  const unsigned long now = millis();

  if (now - lastHeartbeat >= HEARTBEAT_INTERVAL_MS) {
    lastHeartbeat = now;
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
  }

  if (now - lastHello >= HELLO_INTERVAL_MS) {
    lastHello = now;
    Serial.println("Hello from RP2040 over USB serial!");
    fill_solid(statusPixel, NEOPIXEL_COUNT, CRGB::White);
    FastLED.show();
  }
}
