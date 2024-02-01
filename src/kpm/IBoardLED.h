#pragma once

class IBoardLED {
public:
  virtual void initialize() = 0;
  virtual void write(int index, bool value) = 0;
  virtual void toggle(int index) = 0;
};
