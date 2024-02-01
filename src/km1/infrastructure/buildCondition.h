#pragma once

#if defined(ARDUINO_ARCH_RP2040) && !defined(KERMITECORE_MANUAL_STACK_CONFIG)
// default stack configuration for RP2040

#if defined(USE_TINYUSB)
#define KEMRITECORE_USE_USBIOCORE_ADAFRUIT_TINYUSB
#else
#define KEMRITECORE_USE_USBIOCORE_RP2040_ARDUINO_PICO
#endif

#define KERMITECORE_USE_FLASHSTORAGE_RP2040_PICO_SDK

#endif

#if defined(KERMITECORE_USE_FLASHSTORAGE_ADAFRUIT_SPIFLASH)
#include <Adafruit_SPIFlash.h> //tell LDF to depend on this
#endif

#if defined(KEMRITECORE_USE_USBIOCORE_ADAFRUIT_TINYUSB)
#include <Adafruit_TinyUSB.h> //tell LDF to depend on this
#endif

#if defined(KERMITECORE_PRODUCTION_MODE)
const bool buildCondition_kermiteCore_productionMode = true;
#else
const bool buildCondition_kermiteCore_productionMode = false;
#endif
