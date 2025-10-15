#include "serial_ui.h"

#include <Arduino.h>

#include "app_config.h"

void printStartupBanner() {
  Serial.println();
  Serial.println("=============================================");
  Serial.println(" RP2040 / ESP32 CPU Benchmark (QR Code Gen)");
  Serial.println("=============================================");
  Serial.println("USB serial link is ready.");
  Serial.print("Project repo (and QR source): ");
  Serial.println(AppConfig::repoUrl);
  Serial.println();
}
