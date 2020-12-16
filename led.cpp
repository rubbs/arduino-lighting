
#include "led.h"


LedController::LedController() {
}

void LedController::setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
}

void LedController::success() {
  const int maxBrightness = 150;
  fill_solid (leds, NUM_LEDS, CRGB::Green);
  for (int i = 0; i < maxBrightness; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(1);
  }
  for (int i = 0; i < maxBrightness; i++) {
    FastLED.setBrightness(maxBrightness - i);
    FastLED.show();
    delay(1);
  }
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void LedController::show(char r,char g, char b, char brightness){
  FastLED.setBrightness(brightness);
  fill_solid (leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
}