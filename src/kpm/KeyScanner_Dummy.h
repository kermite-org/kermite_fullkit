#pragma once
#include "IKeyScanner.h"

class KeyScanner_Dummy : public IKeyScanner {
public:
  void setKeyIndexBase(int keyIndexBase);
  void setKeyStateListener(KeyStateListenerFn fn);
  void initialize();
  void updateInput();
};
