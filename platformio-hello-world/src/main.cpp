#include <Arduino.h>
#include <FastLED.h>
#include "qrcodegen.h" // Required for QR code generation

// --- Original Configuration ---
constexpr unsigned long BAUD_RATE = 115200;
constexpr unsigned long HELLO_INTERVAL_MS = 1000;
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 500;
constexpr unsigned long SERIAL_WAIT_TIMEOUT_MS = 5000;

constexpr uint8_t NEOPIXEL_PIN = 16;
constexpr uint8_t NEOPIXEL_COUNT = 1;
constexpr uint8_t NEOPIXEL_BRIGHTNESS = 128;
constexpr uint8_t BUTTON_PIN = 0;        // BOOTSEL button is wired to GP0 on the RP2040 Zero
constexpr unsigned long BUTTON_DEBOUNCE_MS = 30;

constexpr char REPO_URL[] = "https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project";
constexpr char HELLO_MESSAGE[] = "Hello from RP2040 over USB serial!";
constexpr CRGB HEARTBEAT_ON_COLOR = CRGB::Aqua;
constexpr CRGB HEARTBEAT_OFF_COLOR = CRGB::Black;
constexpr CRGB HELLO_COLOR = CRGB::White;
constexpr CRGB BENCHMARK_COMPLETE_COLOR = CRGB::Green;

CRGB statusPixel[NEOPIXEL_COUNT];

// --- Original Helper Functions ---
void showStatusPixel(const CRGB& color) {
  fill_solid(statusPixel, NEOPIXEL_COUNT, color);
  FastLED.show();
}

void printStartupBanner() {
  Serial.println();
  Serial.println("=============================================");
  Serial.println(" RP2040 / ESP32 CPU Benchmark (QR Code Gen)");
  Serial.println("=============================================");
  Serial.println("USB serial link is ready.");
  Serial.print("Project repo (and QR source): ");
  Serial.println(REPO_URL);
  Serial.println();
}

// --- NEW: QR Code Benchmark Function ---
// Prints the QR code to the Serial Monitor as ASCII art.
void printQrCode(const uint8_t qrcode[]) {
    int size = qrcodegen_getSize(qrcode);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // Use "██" for a denser look, or "##" / "  " for a sparser one.
            if (qrcodegen_getModule(qrcode, x, y)) {
                Serial.print("██");
            } else {
                Serial.print("  ");
            }
        }
        Serial.println();
    }
}


// --- Main Setup ---
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(BAUD_RATE);

  FastLED.addLeds<WS2812B, NEOPIXEL_PIN, GRB>(statusPixel, NEOPIXEL_COUNT);
  FastLED.setBrightness(NEOPIXEL_BRIGHTNESS);
  showStatusPixel(HELLO_COLOR);

  const unsigned long start = millis();
  while (!Serial && (millis() - start) < SERIAL_WAIT_TIMEOUT_MS) {
    delay(10);
  }

  printStartupBanner();

  // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
  // --- BENCHMARK CODE STARTS HERE ---
  // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#if defined(ARDUINO_ARCH_ESP32)
    // ESP32: Multi-core benchmark implementation
    Serial.println("Starting ESP32 dual-core QR code benchmark...");

    // Task to be run on each core
    auto qrCodeGenerationTask = [](void *pvParameters) {
        uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
        uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
        
        long startTime = micros();
        bool success = qrcodegen_encodeText(REPO_URL, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
            qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
        long timeTaken = micros() - startTime;

        if (success) {
            Serial.printf("Core %d finished in: %ld microseconds.\n", xPortGetCoreID(), timeTaken);
            // To prevent printing the QR code twice, we can use a flag or just let Core 0 print it.
            if(xPortGetCoreID() == 0) {
               printQrCode(qrcode);
            }
        } else {
            Serial.printf("Core %d failed to generate QR code.\n", xPortGetCoreID());
        }
        vTaskDelete(NULL); // End task
    };

    // Create and pin tasks to each core
    xTaskCreatePinnedToCore(qrCodeGenerationTask, "QRTaskCore0", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(qrCodeGenerationTask, "QRTaskCore1", 10000, NULL, 1, NULL, 1);
    
    // Allow tasks time to run and print results before continuing.
    delay(500);


#elif defined(ARDUINO_ARCH_RP2040)
    // RP2040: Single-core benchmark implementation
    Serial.println("Starting RP2040 single-core QR code benchmark...");

    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

    long startTime = micros();
    bool success = qrcodegen_encodeText(REPO_URL, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    long endTime = micros();

    if (success) {
        Serial.println("\n--- Benchmark Result ---");
        Serial.print("Time taken: ");
        Serial.print(endTime - startTime);
        Serial.println(" microseconds");
        Serial.println("------------------------\n");
        printQrCode(qrcode);
    } else {
        Serial.println("QR code generation failed.");
    }

#else
    // Fallback for other boards
    Serial.println("This benchmark is optimized for ESP32 or RP2040.");
    Serial.println("Board not recognized. No benchmark will be run.");
#endif

  // --- BENCHMARK CODE ENDS HERE ---
  Serial.println("\nBenchmark complete. Starting normal operation...");
  Serial.println("---------------------------------------------\n");
  showStatusPixel(BENCHMARK_COMPLETE_COLOR);
  delay(1000); // Pause to show the "complete" color
}


// --- Original Loop ---
// This will run after the benchmark in setup() is complete.
void loop() {
  static unsigned long lastHello = 0;
  static unsigned long lastHeartbeat = 0;
  static bool ledOn = false;
  static uint32_t helloCount = 0;
  static int buttonStableState = HIGH;
  static int buttonLastRead = HIGH;
  static unsigned long lastButtonChange = 0;
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

  // Debounced BOOT button detection (active-low due to INPUT_PULLUP).
  const int buttonReading = digitalRead(BUTTON_PIN);
  if (buttonReading != buttonLastRead) {
    lastButtonChange = now;
    buttonLastRead = buttonReading;
  }

  if ((now - lastButtonChange) >= BUTTON_DEBOUNCE_MS && buttonStableState != buttonReading) {
    buttonStableState = buttonReading;
    if (buttonStableState == LOW) {
      Serial.println("BOOT button pressed (GP0 low)");
    } else {
      Serial.println("BOOT button released (GP0 high)");
    }
  }
}