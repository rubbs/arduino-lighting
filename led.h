#ifndef __LED_H__
#define __LED_H__

#include "FastLED.h"
// LED configuration
// Fastled constants
#define DATA_PIN    2
#define COLOR_ORDER GRB
#define NUM_LEDS    300
#define LED_TYPE    WS2812B
#define BRIGHTNESS  64



class LedController {
  public:
    LedController();
    void setup();
    void success();  // blink code for success
    void show(char r,char g, char b, char brightness);
  
  private:
    CRGB leds[NUM_LEDS];
};


#endif