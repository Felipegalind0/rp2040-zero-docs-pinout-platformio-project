/*
 * Multi-threaded CPU Benchmark for ESP32 and RP2040
 *
 * This sketch benchmarks the CPU performance of an ESP32 or RP2040
 * by timing how long it takes to generate a QR code. The result is
 * printed as ASCII art to the Serial Monitor.
 *
 * For ESP32, it uses both cores in a "race" to see how quickly the
 * QR code can be generated. For RP2040, it runs on a single core.
 *
 * Dependencies:
 *  - qrcodegen library by Project Nayuki:
 *    Install from the Arduino Library Manager.
 */

#ifdef BUILD_QR_TEST

#include <Arduino.h>
#include "qrcodegen.h"

// The string to encode into the QR code. A longer string will
// result in a more complex QR code and a longer generation time.
const char* text = "https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project";

// Function to print the QR code to the Serial Monitor as ASCII art.
void printQrCode(const uint8_t qrcode[]) {
    int size = qrcodegen_getSize(qrcode);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qrcodegen_getModule(qrcode, x, y)) {
                Serial.print("##");
            } else {
                Serial.print("  ");
            }
        }
        Serial.println();
    }
}

#if defined(ARDUINO_ARCH_ESP32)

// For the ESP32, we'll use FreeRTOS to create tasks on each core.

// Task handles are not strictly necessary for this example, but are good practice.
TaskHandle_t qrTask1;
TaskHandle_t qrTask2;

// A struct to hold the results from each core.
struct CoreResult {
    bool success = false;
    long time_us = 0;
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
};

CoreResult resultCore0;
CoreResult resultCore1;

// The task that will be run on each core.
void qrCodeGenerationTask(void *pvParameters) {
    int core = (int)pvParameters;
    long startTime = micros();

    // Create a temporary buffer for the QR code generation.
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];

    bool success = qrcodegen_encodeText(text, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

    long endTime = micros();

    // Store the results.
    if (core == 0) {
        resultCore0.success = success;
        resultCore0.time_us = endTime - startTime;
        memcpy(resultCore0.qrcode, qrcode, sizeof(qrcode));
    } else {
        resultCore1.success = success;
        resultCore1.time_us = endTime - startTime;
        memcpy(resultCore1.qrcode, qrcode, sizeof(qrcode));
    }

    // End the task.
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { }

    Serial.println("Starting ESP32 dual-core QR code benchmark...");

    // Create and pin the tasks to the cores.
    xTaskCreatePinnedToCore(
        qrCodeGenerationTask, "QRTaskCore0", 10000, (void*)0, 1, &qrTask1, 0);

    xTaskCreatePinnedToCore(
        qrCodeGenerationTask, "QRTaskCore1", 10000, (void*)1, 1, &qrTask2, 1);
        
    // Wait for both tasks to complete by checking their results.
    while (resultCore0.time_us == 0 || resultCore1.time_us == 0) {
        delay(10);
    }

    Serial.println("\nBenchmark complete.\n");

    // Print the QR code from the first core that finished.
    if (resultCore0.success) {
        printQrCode(resultCore0.qrcode);
    } else if (resultCore1.success) {
        printQrCode(resultCore1.qrcode);
    } else {
        Serial.println("QR code generation failed on both cores.");
        return;
    }
    
    Serial.println("\n--- Results ---");
    Serial.print("Core 0 time: ");
    Serial.print(resultCore0.time_us);
    Serial.println(" microseconds");

    Serial.print("Core 1 time: ");
    Serial.print(resultCore1.time_us);
    Serial.println(" microseconds");
    Serial.println("-----------------");
}

void loop() {
    // Everything is done in setup().
}

#elif defined(ARDUINO_ARCH_RP2040)

// For the RP2040, we'll run a single-threaded benchmark.

void qrsetup() {
    Serial.begin(115200);
    while (!Serial) { }

    Serial.println("Starting RP2040 single-core QR code benchmark...");

    long startTime = micros();

    // Buffers for QR code generation.
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];

    bool success = qrcodegen_encodeText(text, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

    long endTime = micros();

    Serial.println("\nBenchmark complete.\n");

    if (success) {
        printQrCode(qrcode);
        Serial.println("\n--- Results ---");
        Serial.print("Time taken: ");
        Serial.print(endTime - startTime);
        Serial.println(" microseconds");
        Serial.println("-----------------");
    } else {
        Serial.println("QR code generation failed.");
    }
}

void loop() {
    // Everything is done in setup().
}

#else

// If the board is not an ESP32 or RP2040, print an error.
void setup() {
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println("This benchmark is only for ESP32 or RP2040 boards.");
}

void loop() { }

#endif

#endif // BUILD_QR_TEST