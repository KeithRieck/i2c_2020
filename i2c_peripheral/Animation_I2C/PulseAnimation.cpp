#include "PulseAnimation.h"

PulseAnimation::PulseAnimation() {
  colorCount = 2;
  colorList = (uint32_t*)malloc(colorCount * sizeof(uint32_t));
  colorList[0] = BLUE;
  colorList[1] = YELLOW;
}

PulseAnimation::PulseAnimation(int n) {
  colorCount = n;
  colorList = (uint32_t*)malloc(colorCount * sizeof(uint32_t));
  for (int i=0; i<colorCount; i++) {
    colorList[i] = BLACK;
  }
  colorList[0] = BLUE;
  colorList[1] = YELLOW;
}

void PulseAnimation::setColor(int i, uint32_t c) {
  colorList[i] = c;
}

void PulseAnimation::reset(PixelStrip *strip) {
  strip->begin();
}

void PulseAnimation::draw(PixelStrip *strip) {
  unsigned long t = millis() % time_cycle;
  for (int p = 0; p < strip->numPixels(); p++) {
    int color_num = p % colorCount;
    int time_shift = color_num * (time_cycle / colorCount);
    int brightness = f((t + time_shift) % time_cycle);
    uint32_t c1 = colorList[color_num];
    uint32_t c2 = fadeColor(c1, brightness);
    strip->setPixelColor(p, c2);
  }
}

/**
   Return a brightness value as a function of time.
   The input value is the number of milliseconds into the cycle, from zero to _time_cycle.
   The return value is a brightness value from 0 to 255.
*/
int PulseAnimation::f(unsigned long t) {
  float theta = 6.283 * t / time_cycle;   // Angle in radians
  float s = (sin(theta) + 1.0) / 2.0;     // Value from 0.0 to 1.0
  return s * 256;
}

/**
   Return a color that has been faded by the given brightness.
   The brightness parameter is a number from 0 to 255.
   The output is a new color.
*/
uint32_t PulseAnimation::fadeColor(uint32_t c, int brightness) {
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >>  8);
  uint8_t b = (uint8_t)c;
  r = (uint8_t)( ((int)r * brightness / 256) );
  g = (uint8_t)( ((int)g * brightness / 256) );
  b = (uint8_t)( ((int)b * brightness / 256) );
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
