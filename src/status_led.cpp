#include "status_led.h"

#include <FastLED.h>

#include "app_config.h"

namespace {
CRGB statusPixel[AppConfig::neoPixelCount];
}

void initStatusPixel() {
  FastLED.addLeds<WS2812B, AppConfig::neoPixelPin, GRB>(statusPixel, AppConfig::neoPixelCount);
  FastLED.setBrightness(AppConfig::neoPixelBrightness);
  showStatusPixel(AppConfig::helloColor);
}

void showStatusPixel(const CRGB& color) {
  fill_solid(statusPixel, AppConfig::neoPixelCount, color);
  FastLED.show();
}
