#include "BoardLED.h"

#include <Arduino.h>

BoardLED::BoardLED(int _pin0, int _pin1, int _pin2, bool _invert) {
  pins[0] = _pin0;
  pins[1] = _pin1;
  pins[2] = _pin2;
  invert = _invert;
}

void BoardLED::initialize() {
  for (int i = 0; i < 3; i++) {
    int pin = pins[i];
    if (pin != -1) {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, invert ? HIGH : LOW);
    }
  }
}

void BoardLED::write(int index, bool value) {
  int pin = pins[index];
  if (pin != -1) {
    digitalWrite(pin, value ^ invert);
  }
}

void BoardLED::toggle(int index) {
  int pin = pins[index];
  if (pin != -1) {
    digitalWrite(pin, !digitalRead(pin));
  }
}
