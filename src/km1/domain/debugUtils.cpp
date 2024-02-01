#include "debugUtils.h"
#include "../infrastructure/kprintf.h"
#include <Arduino.h>

void debugUtils_printBytes(uint8_t *buf, int len) {
  for (int i = 0; i < len; i++) {
    kprintf("%02X ", buf[i]);
  }
  kprintf("\n");
}

void debugUtils_printBytesDec(uint8_t *buf, int len) {
  for (int i = 0; i < len; i++) {
    kprintf("%d ", buf[i]);
  }
  kprintf("\n");
}
