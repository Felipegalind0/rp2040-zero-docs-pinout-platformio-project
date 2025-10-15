#include "qr_benchmark.h"

#include <Arduino.h>

#include "app_config.h"
#include "qrcodegen.h"

namespace {
void printQrCode(const uint8_t qrcode[]) {
  const int size = qrcodegen_getSize(qrcode);
  for (int y = 0; y < size; ++y) {
    for (int x = 0; x < size; ++x) {
      Serial.print(qrcodegen_getModule(qrcode, x, y) ? "██" : "  ");
    }
    Serial.println();
  }
}

#if defined(ARDUINO_ARCH_ESP32)
void qrCodeGenerationTask(void* /*pvParameters*/) {
  uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
  uint8_t temp[qrcodegen_BUFFER_LEN_MAX];

  const long startTime = micros();
  const bool success = qrcodegen_encodeText(AppConfig::repoUrl, temp, qrcode, qrcodegen_Ecc_LOW,
                                            qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO,
                                            true);
  const long elapsed = micros() - startTime;

  if (success) {
    Serial.printf("Core %d finished in: %ld microseconds.\n", xPortGetCoreID(), elapsed);
    if (xPortGetCoreID() == 0) {
      printQrCode(qrcode);
    }
  } else {
    Serial.printf("Core %d failed to generate QR code.\n", xPortGetCoreID());
  }

  vTaskDelete(nullptr);
}
#endif
}  // namespace

void runQrBenchmark() {
#if defined(ARDUINO_ARCH_ESP32)
  Serial.println("Starting ESP32 dual-core QR code benchmark...");
  xTaskCreatePinnedToCore(qrCodeGenerationTask, "QRTaskCore0", 10000, nullptr, 1, nullptr, 0);
  xTaskCreatePinnedToCore(qrCodeGenerationTask, "QRTaskCore1", 10000, nullptr, 1, nullptr, 1);
  delay(500);
#elif defined(ARDUINO_ARCH_RP2040)
  Serial.println("Starting RP2040 single-core QR code benchmark...");

  uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
  uint8_t temp[qrcodegen_BUFFER_LEN_MAX];

  const long startTime = micros();
  const bool success = qrcodegen_encodeText(AppConfig::repoUrl, temp, qrcode, qrcodegen_Ecc_LOW,
                                            qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO,
                                            true);
  const long elapsed = micros() - startTime;

  if (success) {
    Serial.println("\n--- Benchmark Result ---");
    Serial.print("Time taken: ");
    Serial.print(elapsed);
    Serial.println(" microseconds");
    Serial.println("------------------------\n");
    printQrCode(qrcode);
  } else {
    Serial.println("QR code generation failed.");
  }
#else
  Serial.println("This benchmark is optimized for ESP32 or RP2040.");
  Serial.println("Board not recognized. No benchmark will be run.");
#endif
}
