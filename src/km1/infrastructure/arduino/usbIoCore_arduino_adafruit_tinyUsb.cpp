#include "../buildCondition.h"
#if defined(KEMRITECORE_USE_USBIOCORE_ADAFRUIT_TINYUSB)

#include "tusb_config.h"
#if CFG_TUD_HID < 2
#error incompatible preprocessor definition, please provide -DCFG_TUD_HID=2
#endif

#include "../kprintf.h"
#include "../system.h"
#include "../usbIoCore.h"
#include "km1/domain/debugUtils.h"
#include <Adafruit_TinyUSB.h>

enum {
  RID_KEYBOARD = 1,
  RID_MOUSE,
  RID_CONSUMER_CONTROL,
};

static const int rawHidDataLength = 64;

static const uint8_t descHidReportShared[] = {
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
  TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(RID_MOUSE)),
  TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL)),
};

static const uint8_t descHidReportGeneric[] = {
  TUD_HID_REPORT_DESC_GENERIC_INOUT(rawHidDataLength),
};

static Adafruit_USBD_HID hidShared(descHidReportShared, sizeof(descHidReportShared), HID_ITF_PROTOCOL_NONE, 2, true);
static Adafruit_USBD_HID hidGeneric(descHidReportGeneric, sizeof(descHidReportGeneric), HID_ITF_PROTOCOL_NONE, 2, true);

static uint8_t rawHidRxBuf[4][rawHidDataLength];
static uint32_t rawHidRxPageCount = 0;

static uint8_t keyboardLedStatus = 0;

static void hidShared_setReportCallback(uint8_t reportId, hid_report_type_t reportType, uint8_t const *buffer, uint16_t bufsize) {
  if (reportId == RID_KEYBOARD && bufsize == 1) {
    keyboardLedStatus = buffer[0];
  }
}

static void hidGeneric_setReportCallback(uint8_t reportId, hid_report_type_t reportType, uint8_t const *buffer, uint16_t bufsize) {
  if (rawHidRxPageCount < 4 && bufsize <= rawHidDataLength) {
    uint8_t *destBuf = rawHidRxBuf[rawHidRxPageCount];
    memset(destBuf, 0, rawHidDataLength);
    memcpy(destBuf, buffer, bufsize);
    rawHidRxPageCount++;
  }
}

void usbIoCore_initialize() {
  USBDevice.setID(0xF055, 0xA579);
  // USBDevice.setID(0xF055, 0xA57A); //for debugging
  USBDevice.setManufacturerDescriptor("Kermite");

  hidShared.setReportCallback(NULL, hidShared_setReportCallback);
  hidShared.begin();

  hidGeneric.setReportCallback(NULL, hidGeneric_setReportCallback);
  hidGeneric.begin();

  while (!USBDevice.mounted()) {
    system_delayMs(1);
  }
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
    kprintf("cannot enqueue hid report (8/8)\n");
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

static void sendKeyboardReport(uint8_t *pReportBytes8) {
  hidShared.sendReport(RID_KEYBOARD, pReportBytes8, 8);
}

static void sendMouseReport(uint8_t *pReportBytes7) {
  hidShared.sendReport(RID_MOUSE, pReportBytes7, 7);
}

static void sendConsumerControlReport(uint8_t *pReportBytes2) {
  hidShared.sendReport(RID_CONSUMER_CONTROL, pReportBytes2, 2);
}

static void sendRawHidReport(uint8_t *pDataBytes64) {
  hidGeneric.sendReport(0, pDataBytes64, rawHidDataLength);
}

static void emitOneReportIfReady() {
  //todo: peek individually
  if (hidShared.ready() && hidGeneric.ready()) {
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

void usbIoCore_rawHid_writeData(uint8_t *pDataBytes64) {
  reportEmitterQueue_push(sendRawHidReport, pDataBytes64, rawHidDataLength);
  emitOneReportIfReady();
}

uint8_t usbIoCore_hidKeyboard_getStatusLedFlags() {
  return keyboardLedStatus;
}

bool usbIoCore_rawHid_readDataIfExists(uint8_t *pDataBytes64) {
  if (rawHidRxPageCount > 0) {
    memcpy(pDataBytes64, rawHidRxBuf[--rawHidRxPageCount], rawHidDataLength);
    return true;
  }
  return false;
}

bool usbIoCore_isConnectedToHost() {
  return USBDevice.ready();
}

void usbIoCore_setProductName(const char *productNameText) {
  USBDevice.setProductDescriptor(productNameText);
}

void usbIoCore_setSerialNumber(const char *serialNumberText) {
  USBDevice.setSerialDescriptor(serialNumberText);
}

void usbIoCore_processUpdate() {
  emitOneReportIfReady();
}

void usbIoCore_stopUsbSerial() {
  Serial.end();
}
#endif
