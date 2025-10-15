#pragma once

#include <FastLED.h>
#include <stdint.h>

namespace AppConfig {

inline constexpr unsigned long baudRate = 115200;
inline constexpr unsigned long helloIntervalMs = 1000;
inline constexpr unsigned long heartbeatIntervalMs = 500;
inline constexpr unsigned long serialWaitTimeoutMs = 5000;

inline constexpr uint8_t neoPixelPin = 16;
inline constexpr uint8_t neoPixelCount = 1;
inline constexpr uint8_t neoPixelBrightness = 128;
inline constexpr uint8_t buttonPin = 0;
inline constexpr unsigned long buttonDebounceMs = 30;

inline constexpr char repoUrl[] = "https://github.com/Felipegalind0/rp2040-zero-docs-pinout-platformio-project";
inline constexpr char helloMessage[] = "Hello from RP2040 over USB serial!";

inline const CRGB heartbeatOnColor = CRGB::Aqua;
inline const CRGB heartbeatOffColor = CRGB::Black;
inline const CRGB helloColor = CRGB::Green;
inline const CRGB benchmarkCompleteColor = CRGB::Green;

}  // namespace AppConfig
