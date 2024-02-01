#pragma once

#include <stdint.h>

uint16_t dataMemory_getCapacity();
uint8_t dataMemory_readByte(uint16_t addr);
void dataMemory_writeByte(uint16_t addr, uint8_t val);
uint16_t dataMemory_readWord(uint16_t addr);
void dataMemory_writeWord(uint16_t addr, uint16_t val);
void dataMemory_readBytes(uint16_t addr, uint8_t *buf, uint16_t len);
void dataMemory_writeBytes(uint16_t addr, uint8_t *buf, uint16_t len);
void dataMemory_clearAllZero();

void dataMemory_initialize();
void dataMemory_processTick();

void dataMemory_setSavingWaitTimeSec(int sec);
