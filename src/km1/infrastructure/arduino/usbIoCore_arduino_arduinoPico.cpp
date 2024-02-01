#include "../buildCondition.h"
#if defined(KEMRITECORE_USE_USBIOCORE_RP2040_ARDUINO_PICO)

#include "../kprintf.h"
#include "RP2040USB.h"
#include "SerialUSB.h"
#include "tusb.h"

void __USBInstallKeyboard() {}
void __USBInstallMouse() {}
void __USBInstallConsumerControl() {}
void __USBInstallSecondHID_RawHID() {}

static __USBDeviceAttributes usbDeviceAttrs = {
  .vendorId = 0xF055,
  .productId = 0xA579,
  // .productId = 0xA57A, //debug
  .manufacturerName = "Kermite",
  .productName = "unnamed keyboard",
  .serialNumberText = "00000000"
};

static const int rawHidDataLength = 64;

static uint8_t hidKeyboardStatusLedFlags = 0;

static uint8_t rawHidRxBuf[4][rawHidDataLength];
static uint32_t rawHidRxPageCount = 0;

// static uint8_t dvals[5]; //debug

void hidSetReportCallbackFn(uint8_t instance, uint8_t reportId, uint8_t reportType, uint8_t const *buffer, uint16_t bufsize) {
  const int instanceShared = __USBGetHIDInstanceIndexForSharedHID();
  const int instanceRawHid = __USBGetHIDInstanceIndexForRawHID();
  if (instance == instanceShared) {
    const int reportIdHidKeyboard = __USBGetKeyboardReportID();
    if (reportId == reportIdHidKeyboard && bufsize == 1) {
      hidKeyboardStatusLedFlags = buffer[0];
    }
  }
  // dvals[0] = 0xAA;
  // dvals[1] = instance;
  // dvals[2] = reportId;
  // dvals[3] = reportType;
  // dvals[4] = bufsize;
  if (instance == instanceRawHid && reportId == 0 && bufsize <= rawHidDataLength) {
    if (rawHidRxPageCount < 4) {
      uint8_t *destBuf = rawHidRxBuf[rawHidRxPageCount];
      memset(destBuf, 0, rawHidDataLength);
      memcpy(destBuf, buffer, bufsize);
      rawHidRxPageCount++;
    }
  }
}

void usbIoCore_initialize() {
  __USBSetDeviceAttributes(usbDeviceAttrs);
  __USBSubscribeHIDSetReportCallback(hidSetReportCallbackFn);
}

//----------
//report emitter queue

typedef void (*ReportEmitterFn)(uint8_t *report);
typedef struct {
  ReportEmitterFn reportEmitterFn;
  uint8_t *reportBytes;
} ReportQueueItem;

static ReportQueueItem *reportQueue[8];
static int reportQueue_wi = 0;
static int reportQueue_ri = 0;

static void reportEmitterQueue_push(ReportEmitterFn fn, uint8_t *report, int len) {
  if (((reportQueue_wi + 1) & 7) == reportQueue_ri) {
    kprintf("cannot enqueue hid report (8/8)n");
  } else {
    uint8_t *reportBytes = new uint8_t[len];
    memcpy(reportBytes, report, len);
    ReportQueueItem *item = new ReportQueueItem();
    item->reportEmitterFn = fn;
    item->reportBytes = reportBytes;
    reportQueue[reportQueue_wi] = item;
    reportQueue_wi = (reportQueue_wi + 1) & 7;
  }
}

static ReportQueueItem *reportEmitterQueue_pop() {
  if (reportQueue_wi != reportQueue_ri) {
    ReportQueueItem *item = reportQueue[reportQueue_ri];
    reportQueue[reportQueue_ri] = nullptr;
    reportQueue_ri = (reportQueue_ri + 1) & 7;
    return item;
  }
  return nullptr;
}

static void reportEmitterQueue_emitOne() {
  ReportQueueItem *item = reportEmitterQueue_pop();
  if (item) {
    item->reportEmitterFn(item->reportBytes);
    delete[] item->reportBytes;
    delete item;
  }
}

//----------

uint8_t usbIoCore_hidKeyboard_getStatusLedFlags() {
  return hidKeyboardStatusLedFlags;
}

static void sendKeyboardReport(uint8_t *pReportBytes8) {
  const int instance = __USBGetHIDInstanceIndexForSharedHID();
  int reportId = __USBGetKeyboardReportID();
  tud_hid_n_report(instance, reportId, pReportBytes8, 8);
}

static void sendMouseReport(uint8_t *pReportBytes7) {
  const int instance = __USBGetHIDInstanceIndexForSharedHID();
  int reportId = __USBGetMouseReportID();
  tud_hid_n_report(instance, reportId, pReportBytes7, 7);
}

static void sendConsumerControlReport(uint8_t *pReportBytes2) {
  const int instance = __USBGetHIDInstanceIndexForSharedHID();
  const int reportId = __USBGetConsumerControlReportID();
  tud_hid_n_report(instance, reportId, pReportBytes2, 2);
}

static void sendRawHidReport(uint8_t *pDataBytes64) {
  const int instance = __USBGetHIDInstanceIndexForRawHID();
  tud_hid_n_report(instance, 0, pDataBytes64, rawHidDataLength);
}

static void emitOneReportIfReady() {
  const int instance1 = __USBGetHIDInstanceIndexForSharedHID();
  const int instance2 = __USBGetHIDInstanceIndexForRawHID();
  //todo: peek individually
  if (tud_hid_n_ready(instance1) && tud_hid_n_ready(instance2)) {
    reportEmitterQueue_emitOne();
  }
}

void usbIoCore_hidKeyboard_writeReport(uint8_t *pReportBytes8) {
  reportEmitterQueue_push(sendKeyboardReport, pReportBytes8, 8);
  emitOneReportIfReady();
}

void usbIoCore_hidMouse_writeReport(uint8_t *pReportBytes7) {
  reportEmitterQueue_push(sendMouseReport, pReportBytes7, 7);
  emitOneReportIfReady();
}

void usbIoCore_hidConsumerControl_writeReport(uint8_t *pReportBytes2) {
  reportEmitterQueue_push(sendConsumerControlReport, pReportBytes2, 2);
  emitOneReportIfReady();
}

bool usbIoCore_rawHid_writeData(uint8_t *pDataBytes64) {
  // kprintf("debug:%d %d %d %d %d\n", dvals[0], dvals[1], dvals[2], dvals[3], dvals[4]);
  reportEmitterQueue_push(sendRawHidReport, pDataBytes64, rawHidDataLength);
  emitOneReportIfReady();
  return true;
}

bool usbIoCore_rawHid_readDataIfExists(uint8_t *pDataBytes64) {
  if (rawHidRxPageCount > 0) {
    memcpy(pDataBytes64, rawHidRxBuf[--rawHidRxPageCount], rawHidDataLength);
    return true;
  }
  return false;
}

bool usbIoCore_isConnectedToHost() {
  return tud_hid_ready();
}

void usbIoCore_setProductName(const char *productNameText) {
  usbDeviceAttrs.productName = productNameText;
}

void usbIoCore_setSerialNumber(const char *serialNumberText) {
  usbDeviceAttrs.serialNumberText = serialNumberText;
}

void usbIoCore_processUpdate() {
  emitOneReportIfReady();
}

void usbIoCore_stopUsbSerial() {
  //not supported
  // Serial.end();
}

#endif
