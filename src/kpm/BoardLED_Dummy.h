#pragma once
#include "IBoardLED.h"

class BoardLED_Dummy : public IBoardLED {
public:
  void initialize();
  void write(int index, bool value);
  void toggle(int index);
};
