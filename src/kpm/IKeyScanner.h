#pragma once

typedef void (*KeyStateListenerFn)(int keyIndex, bool pressed);

class IKeyScanner {
public:
  virtual void setKeyIndexBase(int keyIndexBase) = 0;
  virtual void setKeyStateListener(KeyStateListenerFn fn) = 0;
  virtual void initialize() = 0;
  virtual void updateInput() = 0;
};
