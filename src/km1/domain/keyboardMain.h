#pragma once

#include "kmTypes.h"

typedef struct {
  void (*customParameterHandlerOverride)(uint8_t slotIndex, uint8_t value);
  void (*customParameterHandlerExtended)(uint8_t slotIndex, uint8_t value);
  // void (*customCommandHandler)(uint8_t commandIndex);
  void (*layerStateChanged)(uint16_t layerStateFlags);
  void (*keyStateChanged)(uint8_t keyIndex, bool isDown);
} KeyboardCallbackSet;

#define KEYINDEX_NONE 255

typedef struct {
  uint16_t layerStateFlags;
  const uint8_t *hidReportBuf;
  uint8_t pressedKeyIndex;
  // bool isSplitSlave;
  // bool optionInvertSide;
  uint8_t hostKeyboardStateFlags;
} KeyboardMainExposedState;

extern KeyboardMainExposedState keyboardMain_exposedState;
// uint8_t *keyboardMain_getScanSlotFlags();
// uint8_t *keyboardMain_getInputScanSlotFlags();

// void keyboardMain_setAsSplitSlave();
void keyboardMain_setCallbacks(KeyboardCallbackSet *_callbacks);
void keyboardMain_initialize();
void keyboardMain_feedKeyState(int keyIndex, bool pressed);
// void keyboardMain_updateKeyScanners();
// void keyboardMain_updatePointingDevice();
void keyboardMain_processKeyInputUpdate();
// void keyboardMain_updateKeyIndicatorLed();
// void keyboardMain_updateInputSlotIndicatorLed();
// void keyboardMain_updateRgbLightingModules(uint32_t tick);
// void keyboardMain_updateOledDisplayModule(uint32_t tick);
// void keyboardMain_updateHostKeyboardStatusOutputModule();
// void keyboardMain_taskFlashHeartbeatLed();
void keyboardMain_processUpdate();
void keyboardMain_setKeySlotStateChangedCallback(void (*callback)(uint8_t slotIndex, bool isDown));

// void keyboardMain_useKeyScanner(void (*_keyScannerUpdateFunc)(uint8_t *keyStateBitFlags));
// void keyboardMain_usePointingDevice(void (*_pointingDeviceUpdateFunc)(int8_t *outDeltaX, int8_t *outDeltaY));
// void keyboardMain_setKeyIndexTable(const int8_t *_scanIndexToKeyIndexMap);
// void keyboardMain_useRgbLightingModule(void (*_updateFn)(void));
// void keyboardMain_useOledDisplayModule(void (*_updateFn)(void));
// void keyboardMain_useHostKeyboardStatusOutputModule(void (*_updateFn)(void));
