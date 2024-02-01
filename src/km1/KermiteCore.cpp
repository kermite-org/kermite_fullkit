#include "KermiteCore.h"
#include "domain/dataMemory.h"
#include "domain/firmwareMetadata.h"
#include "domain/keyboardMain.h"
#include "infrastructure/buildCondition.h"
#include "infrastructure/kprintf.h"
#include "infrastructure/usbIoCore.h"

KermiteCore::KermiteCore() { firmwareMetadata_initialize(); }

void KermiteCore::setKeyboardName(const char *keyboardName) {
  firmwareMetaData_setKeyboardName(keyboardName);
}

void KermiteCore::enableDebugLogging() { kprintf_turnOnDebugLogging(); }

void KermiteCore::begin() {
  if (buildCondition_kermiteCore_productionMode) {
    usbIoCore_stopUsbSerial();
  }
  keyboardMain_initialize();
}

void KermiteCore::feedKeyState(int keyIndex, bool pressed) {
  keyboardMain_feedKeyState(keyIndex, pressed);
}

void KermiteCore::processUpdate() {
  keyboardMain_processUpdate();
  usbIoCore_processUpdate();
}

void KermiteCore::setFlashSavingWaitTimeSec(int sec) {
  dataMemory_setSavingWaitTimeSec(sec);
}

void KermiteCore::setProductionMode() { usbIoCore_stopUsbSerial(); }
