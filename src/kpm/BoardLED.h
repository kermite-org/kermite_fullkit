#pragma once
#include "IBoardLED.h"

class BoardLED : public IBoardLED {
private:
  bool invert;
  int pins[3];

public:
  BoardLED(int _pin0, int _pin1 = -1, int _pin2 = -1, bool _invert = false);
  void initialize();
  void write(int index, bool value);
  void toggle(int index);
};
