#pragma once

#include "kmTypes.h"

uint16_t keyCodeTranslator_mapLogicalKeyToHidKeyCode(uint8_t logicalKey, bool isSecondaryLayout);
const char *keyCodeTranslator_mapLogicalKeyToKeyText(uint8_t logicalKey);
