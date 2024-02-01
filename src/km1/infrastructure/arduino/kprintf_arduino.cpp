#include "../kprintf.h"
#include <Arduino.h>
#include <stdarg.h>

#ifdef KERMITE_CORE_DEBUG_LOG
bool debugLogEnabled = true;
#else
bool debugLogEnabled = false;
#endif

void kprintf_turnOnDebugLogging() {
  debugLogEnabled = true;
}

int kprintf(const char *format, ...) {
  if (!debugLogEnabled) {
    return 0;
  }
  auto &serial = Serial;

  //copied from
  //https://github.com/earlephilhower/ArduinoCore-API/blob/a3ac9d18fd85aec34482e9ea9fcf2a5194563686/api/Print.cpp#L239
  va_list arg;
  va_start(arg, format);
  char temp[64];
  char *buffer = temp;
  size_t len = vsnprintf(temp, sizeof(temp), format, arg);
  va_end(arg);
  if (len > sizeof(temp) - 1) {
    buffer = new char[len + 1];
    if (!buffer) {
      return 0;
    }
    va_start(arg, format);
    vsnprintf(buffer, len + 1, format, arg);
    va_end(arg);
  }
  len = serial.write((const uint8_t *)buffer, len);
  if (buffer != temp) {
    delete[] buffer;
  }
  return len;
}
