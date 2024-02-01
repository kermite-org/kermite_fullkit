#pragma once
#include "IBoardLED.h"
#include <Adafruit_NeoPixel.h>

class BoardLED_NeoPixel : public IBoardLED {
private:
  Adafruit_NeoPixel neoPixel;
  uint8_t brightness;
  bool states[3];
  int powerPin;
  void updateLedColor();

public:
  BoardLED_NeoPixel(int pin, int _brightness = 0xFF, int _powerPin = -1);
  void initialize();
  void write(int index, bool value);
  void toggle(int index);
};
