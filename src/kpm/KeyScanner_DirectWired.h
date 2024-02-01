#pragma once
#include "IKeyScanner.h"
#include <stdint.h>

class KeyScanner_DirectWired : public IKeyScanner {
private:
  int numPins;
  const uint8_t *pins;
  int keyIndexBase;
  KeyStateListenerFn keyStateListener;
  bool *inputKeyStates;

public:
  bool *keyStates;
  KeyScanner_DirectWired(int _numPins, const uint8_t *_pins);

  void setKeyIndexBase(int _keyIndexBase);
  void setKeyStateListener(KeyStateListenerFn fn);
  void initialize();
  void updateInput();
};
