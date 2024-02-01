#include "KeyScanner_KeyMatrix.h"
#include <Arduino.h>

const int *allocatePins_i32(const uint8_t *pins_u8, int numPins) {
  int *pins_i32 = new int[numPins];
  for (int i = 0; i < numPins; i++) {
    pins_i32[i] = pins_u8[i];
  }
  return pins_i32;
}

KeyScanner_KeyMatrix::KeyScanner_KeyMatrix(
    const int *_columnPins, const int *_rowPins, int _numColumns, int _numRows) {
  columnPins = _columnPins;
  rowPins = _rowPins;
  numColumns = _numColumns;
  numRows = _numRows;
  keyIndexBase = 0;
  keyStateListener = nullptr;
  inputKeyStates = new bool[numColumns * numRows];
  keyStates = new bool[numColumns * numRows];
}

KeyScanner_KeyMatrix::KeyScanner_KeyMatrix(
    const uint8_t *_columnPins, const uint8_t *_rowPins, int _numColumns, int _numRows) {
  columnPins = allocatePins_i32(_columnPins, _numColumns);
  rowPins = allocatePins_i32(_rowPins, _numRows);
  numColumns = _numColumns;
  numRows = _numRows;
  keyIndexBase = 0;
  keyStateListener = nullptr;
  inputKeyStates = new bool[numColumns * numRows];
  keyStates = new bool[numColumns * numRows];
}

void KeyScanner_KeyMatrix::setKeyIndexBase(int _keyIndexBase) {
  keyIndexBase = _keyIndexBase;
}

void KeyScanner_KeyMatrix::setKeyStateListener(KeyStateListenerFn fn) {
  keyStateListener = fn;
}

void KeyScanner_KeyMatrix::initialize() {
  for (int i = 0; i < numColumns; i++) {
    pinMode(columnPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < numRows; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
}

void KeyScanner_KeyMatrix::updateInput() {
  for (int row = 0; row < numRows; row++) {
    int rowPin = rowPins[row];
    pinMode(rowPin, OUTPUT);
    digitalWrite(rowPin, LOW);
    for (int col = 0; col < numColumns; col++) {
      int keyIndex = row * numColumns + col;
      inputKeyStates[keyIndex] = digitalRead(columnPins[col]) == LOW;
    }
    pinMode(rowPin, INPUT_PULLUP);
  }
  int numKeySlots = numColumns * numRows;
  for (int i = 0; i < numKeySlots; i++) {
    if (keyStateListener) {
      bool curr = keyStates[i];
      bool next = inputKeyStates[i];
      if (!curr && next) {
        keyStateListener(keyIndexBase + i, true);
      }
      if (curr && !next) {
        keyStateListener(keyIndexBase + i, false);
      }
    }
    keyStates[i] = inputKeyStates[i];
  }
}
