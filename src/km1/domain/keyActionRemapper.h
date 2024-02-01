#pragma once

#include "kmTypes.h"

void keyActionRemapper_setupDataReader();
uint16_t keyActionRemapper_translateKeyOperation(uint16_t opWord, uint8_t wiringMode);
