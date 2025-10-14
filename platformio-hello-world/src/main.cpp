#include <Arduino.h>
#include <FastLED.h>

constexpr unsigned long BAUD_RATE = 115200;
constexpr unsigned long HELLO_INTERVAL_MS = 1000;
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 500;
constexpr unsigned long SERIAL_WAIT_TIMEOUT_MS = 5000;

constexpr uint8_t NEOPIXEL_PIN = 16;
constexpr uint8_t NEOPIXEL_COUNT = 1;
constexpr uint8_t NEOPIXEL_BRIGHTNESS = 128;

constexpr char REPO_URL[] = "https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project";
constexpr char HELLO_MESSAGE[] = "Hello from RP2040 over USB serial!";
constexpr CRGB HEARTBEAT_ON_COLOR = CRGB::Aqua;
constexpr CRGB HEARTBEAT_OFF_COLOR = CRGB::Black;
constexpr CRGB HELLO_COLOR = CRGB::White;

CRGB statusPixel[NEOPIXEL_COUNT];

void showStatusPixel(const CRGB& color) {
  fill_solid(statusPixel, NEOPIXEL_COUNT, color);
  FastLED.show();
}

void printStartupBanner() {
  Serial.println();
  Serial.println("Raspberry Pi Pico says hello!");
  Serial.println("USB serial link is ready.");
  Serial.print("Project repo: ");
  Serial.println(REPO_URL);
  Serial.println();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(BAUD_RATE);

  FastLED.addLeds<WS2812B, NEOPIXEL_PIN, GRB>(statusPixel, NEOPIXEL_COUNT);
  FastLED.setBrightness(NEOPIXEL_BRIGHTNESS);
  showStatusPixel(HELLO_COLOR);

  const unsigned long start = millis();
  while (!Serial && (millis() - start) < SERIAL_WAIT_TIMEOUT_MS) {
    delay(10);
  }

  printStartupBanner();
}

void loop() {
  static unsigned long lastHello = 0;
  static unsigned long lastHeartbeat = 0;
  static bool ledOn = false;
  static uint32_t helloCount = 0;
  const unsigned long now = millis();

  if (now - lastHeartbeat >= HEARTBEAT_INTERVAL_MS) {
    lastHeartbeat = now;
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
    showStatusPixel(ledOn ? HEARTBEAT_ON_COLOR : HEARTBEAT_OFF_COLOR);
  }

  if (now - lastHello >= HELLO_INTERVAL_MS) {
    lastHello = now;
    ++helloCount;
    Serial.print(HELLO_MESSAGE);
    Serial.print(" (count: ");
    Serial.print(helloCount);
    Serial.println(')');
    showStatusPixel(HELLO_COLOR);
  }
}
