#pragma once

#include "kmTypes.h"

typedef struct {
  uint8_t dataHeader[5];
  char projectId[7];
  char variationId[3];
  char deviceInstanceCode[5];
  char keyboardName[33];
} CommonFirmwareMetadata;

extern CommonFirmwareMetadata commonFirmwareMetadata;

typedef struct {
  char firmwareId[7];
} CommonFirmwareMetadataSecond;

extern CommonFirmwareMetadataSecond commonFirmwareMetadataSecond;

void firmwareMetadata_initialize();
void firmwareMetaData_setKeyboardName(const char *keyboardName);
void firmwareMetaData_setFirmwareId(const char *firmwareId);
