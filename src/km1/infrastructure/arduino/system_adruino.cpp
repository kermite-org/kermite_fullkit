#include "../system.h"
#include <Arduino.h>

uint32_t system_getSystemTimeMs() {
  return millis();
}

void system_delayMs(int ms) {
  delay(ms);
}

void system_delayUs(int us) {
  delayMicroseconds(us);
}

