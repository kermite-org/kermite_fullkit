

#include "dataMemory.h"
#include "../infrastructure/flashPersistSector.h"
#include "../infrastructure/kprintf.h"
#include <string.h>

static const uint32_t persistDataSize = flashPersistSector_DataSize;
static uint8_t ramData[persistDataSize];
static uint32_t saveCount = 0;

static int savingWaitTimeSec = 5;

static void loadRamDataFromFlash() {
  flashPersistSector_read(ramData);
}

static void storeRamDataToFlash() {
  bool written = flashPersistSector_write(ramData);
  if (written) {
    kprintf("data saved.\n");
  }
}

static void saveLazy() {
  saveCount = savingWaitTimeSec * 1000;
}

static void processSaving() {
  if (saveCount > 0) {
    saveCount--;
    if (saveCount == 0) {
      storeRamDataToFlash();
    }
  }
}

uint16_t dataMemory_getCapacity() {
  return persistDataSize;
}

uint8_t dataMemory_readByte(uint16_t addr) {
  return ramData[addr];
}

uint16_t dataMemory_readWord(uint16_t addr) {
  uint8_t lo = ramData[addr];
  uint8_t hi = ramData[addr + 1];
  return (hi << 8) | lo;
}

void dataMemory_readBytes(uint16_t addr, uint8_t *buf, uint16_t len) {
  memcpy(buf, ramData + addr, len);
}

void dataMemory_writeByte(uint16_t addr, uint8_t val) {
  ramData[addr] = val;
  saveLazy();
}

void dataMemory_writeWord(uint16_t addr, uint16_t val) {
  ramData[addr] = val & 0xFF;
  ramData[addr + 1] = (val >> 8) & 0xFF;
  saveLazy();
}

void dataMemory_writeBytes(uint16_t addr, uint8_t *buf, uint16_t len) {
  memcpy(ramData + addr, buf, len);
  saveLazy();
}

void dataMemory_clearAllZero() {
  memset(ramData, 0, persistDataSize);
  saveLazy();
}

void dataMemory_initialize() {
  loadRamDataFromFlash();
}

void dataMemory_processTick() {
  processSaving();
}

void dataMemory_setSavingWaitTimeSec(int sec) {
  savingWaitTimeSec = sec;
}
