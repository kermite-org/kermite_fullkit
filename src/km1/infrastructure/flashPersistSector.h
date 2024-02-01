#pragma once
#include "buildCondition.h"
#include <stdint.h>

const uint32_t flashPersistSector_DataSize = 4096;

void flashPersistSector_read(uint8_t *bytes4096);
bool flashPersistSector_write(uint8_t *bytes4096);
