#pragma once
#include "IKeyScanner.h"
#include <stdint.h>

struct _EncoderInternalState;
typedef struct _EncoderInternalState EncoderInternalState;

class KeyScanner_Encoders : public IKeyScanner {
private:
  EncoderInternalState *encoderStates[4];
  bool moduleActive;
  int numEncoders;
  int numOutputKeys;
  const uint8_t *pins;
  int keyIndexBase;
  KeyStateListenerFn keyStateListener;
  bool *inputKeyStates;

public:
  bool *keyStates;
  KeyScanner_Encoders(int _numPins, const uint8_t *_pins);

  void setKeyIndexBase(int _keyIndexBase);
  void setKeyStateListener(KeyStateListenerFn fn);
  void initialize();
  void updateInput();
};
