#include "../buildCondition.h"
#if defined(KERMITECORE_USE_USBIOCORE_DUMMY_IMPL)

#include "../usbIoCore.h"

void usbIoCore_initialize() {}

void usbIoCore_hidKeyboard_writeReport(uint8_t *pReportBytes8) {}

uint8_t usbIoCore_hidKeyboard_getStatusLedFlags() {
  return 0;
}

void usbIoCore_hidMouse_writeReport(uint8_t *pReportBytes7) {}

void usbIoCore_hidConsumerControl_writeReport(uint8_t *pReportBytes2) {}

void usbIoCore_rawHid_writeData(uint8_t *pDataBytes63) {}

bool usbIoCore_rawHid_readDataIfExists(uint8_t *pDataBytes63) {
  return true;
}

bool usbIoCore_isConnectedToHost() {
  return true;
}

void usbIoCore_setProductName(const char *productNameText) {}

void usbIoCore_setSerialNumber(const char *serialNumberText) {}

void usbIoCore_processUpdate() {}

void usbIoCore_stopUsbSerial() {}
#endif
