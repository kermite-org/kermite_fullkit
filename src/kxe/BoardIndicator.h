#pragma once

class BoardIndicator {
public:
  BoardIndicator(int boardLedType);
  void initialize();
  void update();
  void write(int index, bool value);
};
