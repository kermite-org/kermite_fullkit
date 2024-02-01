#include "../buildCondition.h"
#if defined(KERMITECORE_USE_FLASHSTORAGE_RP2040_PICO_SDK)

#include "../flashPersistSector.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <string.h>

extern uint8_t _FS_start;
extern uint8_t _FS_end;

static const uint32_t fileSystemAreaAddr = ((uint32_t)&_FS_start);
static const uint32_t fileSystemAreaSize = ((uint32_t)(&_FS_end - &_FS_start));

static const int persistDataSize = FLASH_SECTOR_SIZE;
static const uint32_t persistSectorAddr = fileSystemAreaAddr + fileSystemAreaSize - persistDataSize;

void flashPersistSector_read(uint8_t *bytes4096) {
  if (fileSystemAreaSize > 0) {
    uint8_t *persistSectorBuffer = (uint8_t *)persistSectorAddr;
    memcpy(bytes4096, persistSectorBuffer, persistDataSize);
  }
}

bool flashPersistSector_write(uint8_t *bytes4096) {
  if (fileSystemAreaSize > 0) {
    uint32_t status = save_and_disable_interrupts();
    uint32_t flashTargetOffset = persistSectorAddr - XIP_BASE;
    flash_range_erase(flashTargetOffset, FLASH_SECTOR_SIZE);
    flash_range_program(flashTargetOffset, bytes4096, FLASH_SECTOR_SIZE);
    restore_interrupts(status);
    return true;
  }
  return false;
}

#endif
