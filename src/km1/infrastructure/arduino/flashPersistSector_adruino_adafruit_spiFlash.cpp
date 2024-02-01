#include "../buildCondition.h"
#if defined(KERMITECORE_USE_FLASHSTORAGE_ADAFRUIT_SPIFLASH)

#include "../flashPersistSector.h"
#include "../kprintf.h"
#include <Adafruit_FlashTransport.h>
#include <Adafruit_SPIFlash.h>

//Use latter 4k bytes of flash data area to store keymapping and configurations for Kermite
//A user application can still use the space until (filesystem_size - 4kB)

#if defined(ARDUINO_ARCH_RP2040)
static Adafruit_FlashTransport_RP2040 flashTransport;
#elif defined(KERMITECORE_FLASHSTORAGE_ADAFRUIT_SPIFLASH__QSPI_EXTERNAL_FLASH_DEVICE)
Adafruit_FlashTransport_QSPI flashTransport;
#endif

static Adafruit_SPIFlash flash(&flashTransport);

static const int persistDataSize = flashPersistSector_DataSize;
static int32_t locationOffset = -1;

#define P25Q16H                                                         \
  {                                                                     \
    .total_size = (1UL << 21), /* 2 MiB */                              \
        .start_up_time_us = 5000, .manufacturer_id = 0x85,              \
    .memory_type = 0x60, .capacity = 0x15, .max_clock_speed_mhz = 85,   \
    .quad_enable_bit_mask = 0x02, .has_sector_protection = false,       \
    .supports_fast_read = true, .supports_qspi = true,                  \
    .supports_qspi_writes = true, .write_status_register_split = false, \
    .single_status_byte = false, .is_fram = false,                      \
  }

static void beginFlash() {
#ifdef KERMITECORE_FLASHSTORAGE_ADAFRUIT_SPIFLASH__QSPI_EXTERNAL_FLASH_DEVICE
  static const SPIFlash_Device_t spiFlashDevice = KERMITECORE_FLASHSTORAGE_ADAFRUIT_SPIFLASH__QSPI_EXTERNAL_FLASH_DEVICE;
  flash.begin(&spiFlashDevice);
#else
  flash.begin();
#endif
}

void ensureInitialized() {
  static bool initialized = false;
  if (!initialized) {
    beginFlash();
    uint32_t fsSize = flash.size();
    if (fsSize >= persistDataSize) {
      locationOffset = fsSize - persistDataSize;
    }
    initialized = true;
  }
}

void flashPersistSector_read(uint8_t *bytes4096) {
  ensureInitialized();
  if (locationOffset >= 0) {
    flash.readBuffer(locationOffset, bytes4096, persistDataSize);
  }
}
bool flashPersistSector_write(uint8_t *bytes4096) {
  ensureInitialized();
  if (locationOffset >= 0) {
    flash.eraseSector(locationOffset / 4096);
    flash.writeBuffer(locationOffset, bytes4096, persistDataSize);
    return true;
  }
  return false;
}

#endif
