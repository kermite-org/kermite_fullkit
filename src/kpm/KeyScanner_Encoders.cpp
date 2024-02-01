#include "KeyScanner_Encoders.h"
#include <Arduino.h>

struct _EncoderInternalState {
  int pinA;
  int pinB;
  uint8_t bin;
  int dir;
  volatile int value;
  int holdKeyIndex;
};

#define ENC_ERR 100

static const int encoderDeltaTable[16] = {
  0, -1, 1, ENC_ERR,
  1, 0, ENC_ERR, -1,
  -1, ENC_ERR, 0, 1,
  ENC_ERR, 1, -1, 0
};

static void onPinChange(void *_es) {
  EncoderInternalState *es = (EncoderInternalState *)_es;
  int a = digitalRead(es->pinA) == LOW ? 1 : 0;
  int b = digitalRead(es->pinB) == LOW ? 1 : 0;

  uint8_t prev_bin = es->bin;
  es->bin = ((a << 1) | b) & 0b11;

  int tableIndex = ((prev_bin << 2) | es->bin) & 0x0F;
  int delta = encoderDeltaTable[tableIndex];

  if (delta == ENC_ERR) {
    // delta = (es->dir) * 2;
    delta = 0;
  } else {
    es->dir = delta;
  }
  es->value += delta;
}

KeyScanner_Encoders::KeyScanner_Encoders(int _numPins, const uint8_t *_pins) {
  bool pinsValid = (2 <= _numPins && _numPins <= 8);
  if (!pinsValid) {
    moduleActive = false;
    return;
  }
  //ピン数が奇数の場合は末尾のピンを無視して偶数個のピンに対応する
  //エンコーダのインスタンスを確保する
  numEncoders = _numPins / 2;
  numOutputKeys = numEncoders * 2;
  for (int i = 0; i < numEncoders; i++) {
    int pinA = _pins[i * 2];
    int pinB = _pins[i * 2 + 1];
    encoderStates[i] = new EncoderInternalState{
      .pinA = pinA,
      .pinB = pinB,
      .bin = 0,
      .dir = 0,
      .value = 0,
      .holdKeyIndex = -1
    };
  }
  keyIndexBase = 0;
  keyStateListener = nullptr;
  inputKeyStates = new bool[numOutputKeys];
  keyStates = new bool[numOutputKeys];
  moduleActive = true;
}

void KeyScanner_Encoders::setKeyIndexBase(int _keyIndexBase) {
  keyIndexBase = _keyIndexBase;
}

void KeyScanner_Encoders::setKeyStateListener(KeyStateListenerFn fn) {
  keyStateListener = fn;
}

void KeyScanner_Encoders::initialize() {
  if (!moduleActive) {
    return;
  }
  for (int i = 0; i < numEncoders; i++) {
    auto es = encoderStates[i];
    pinMode(es->pinA, INPUT_PULLUP);
    pinMode(es->pinB, INPUT_PULLUP);
    attachInterruptParam(es->pinA, onPinChange, CHANGE, es);
    attachInterruptParam(es->pinB, onPinChange, CHANGE, es);
  }
}

void KeyScanner_Encoders::updateInput() {
  if (!moduleActive) {
    return;
  }
  for (int i = 0; i < numEncoders; i++) {
    auto es = encoderStates[i];
    if (es->holdKeyIndex != -1) {
      //キー押下状態を出力中ならそれを解除
      inputKeyStates[es->holdKeyIndex] = false;
      es->holdKeyIndex = -1;
    } else {
      //エンコーダの値を読み出して、変化があればキー押下状態を出力
      int diff = es->value;
      if (diff != 0) {
        int keyIndex = (i * 2) + (diff < 0 ? 0 : 1);
        inputKeyStates[keyIndex] = true;
        es->holdKeyIndex = keyIndex;
      }
      es->value = 0;
    }
  }

  for (int i = 0; i < numOutputKeys; i++) {
    if (keyStateListener) {
      bool curr = keyStates[i];
      bool next = inputKeyStates[i];
      int keyIndex = keyIndexBase + i;
      if (!curr && next) {
        keyStateListener(keyIndex, true);
      }
      if (curr && !next) {
        keyStateListener(keyIndex, false);
      }
    }
    keyStates[i] = inputKeyStates[i];
  }
}
