#include "KeyScanner_DirectWired.h"
#include <Arduino.h>

KeyScanner_DirectWired::KeyScanner_DirectWired(int _numPins, const uint8_t *_pins) {
  numPins = _numPins;
  pins = _pins;
  keyStateListener = nullptr;
  keyIndexBase = 0;
  inputKeyStates = new bool[numPins];
  keyStates = new bool[numPins];
}

void KeyScanner_DirectWired::setKeyIndexBase(int _keyIndexBase) {
  keyIndexBase = _keyIndexBase;
}

void KeyScanner_DirectWired::setKeyStateListener(KeyStateListenerFn fn) {
  keyStateListener = fn;
}

void KeyScanner_DirectWired::initialize() {
  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void KeyScanner_DirectWired::updateInput() {
  for (int i = 0; i < numPins; i++) {
    inputKeyStates[i] = digitalRead(pins[i]) == LOW;
  }
  for (int i = 0; i < numPins; i++) {
    if (keyStateListener) {
      bool curr = keyStates[i];
      bool next = inputKeyStates[i];
      if (!curr && next) {
        keyStateListener(i, true);
      }
      if (curr && !next) {
        keyStateListener(i, false);
      }
    }
    keyStates[i] = inputKeyStates[i];
  }
}
