#include "BoardLED_NeoPixel.h"
#include <Arduino.h>

BoardLED_NeoPixel::BoardLED_NeoPixel(int pin, int _brightness, int _powerPin)
    : neoPixel(1, pin, NEO_GRB) {
  brightness = _brightness;
  powerPin = _powerPin;
  states[0] = states[1] = states[2] = false;
}

void BoardLED_NeoPixel::initialize() {
  if (powerPin != -1) {
    pinMode(powerPin, OUTPUT);
    digitalWrite(powerPin, HIGH);
  }
  neoPixel.setBrightness(brightness);
  neoPixel.begin();
}

void BoardLED_NeoPixel::updateLedColor() {
  int col = 0;
  if (states[0]) {
    col |= 0xFF0000;
  }
  if (states[1]) {
    col |= 0x00FF00;
  }
  if (states[2]) {
    col |= 0x0000FF;
  }

  neoPixel.clear();
  neoPixel.setPixelColor(0, col);
  neoPixel.show();
}

void BoardLED_NeoPixel::write(int index, bool value) {
  states[index] = value;
  updateLedColor();
}

void BoardLED_NeoPixel::toggle(int index) {
  states[index] = !states[index];
  updateLedColor();
}
