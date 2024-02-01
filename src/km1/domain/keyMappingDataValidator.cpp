#include "keyMappingDataValidator.h"
#include "../base/utils.h"
#include "../infrastructure/kprintf.h"
#include "dataMemory.h"
#include "dataStorage.h"
#include "versionDefinitions.h"
#include <stdio.h>

//----------------------------------------------------------------------

#define decode_byte(p) (*(p))
#define decode_word_be(p) ((*(p) << 8) | (*(p + 1)))

#define KeyMappingDataHeaderLength 5

static uint8_t storageTempBuf[KeyMappingDataHeaderLength];

bool keyMappingDataValidator_checkBinaryProfileDataHeader() {
  uint16_t addrKeyMappingDataHeader = dataStorage_getDataAddress_profileData_profileHeader();
  uint16_t keyMappingDataBodyLengthMax = dataStorage_getKeyMappingDataCapacity();
  bool storageHeaderValid = false;
  if (addrKeyMappingDataHeader && keyMappingDataBodyLengthMax) {
    dataMemory_readBytes(addrKeyMappingDataHeader, storageTempBuf, KeyMappingDataHeaderLength);
    uint8_t *p = storageTempBuf;
    uint8_t logicModelType = decode_byte(p + 0);
    uint8_t configStorageFormatRevision = decode_byte(p + 1);
    uint8_t profileBinaryFormatRevision = decode_byte(p + 2);
    uint8_t numKeys = decode_byte(p + 3);
    uint8_t numLayers = decode_byte(p + 4);
    // kprintf("%d %d %d %d %d\n", logicModelType, configStorageFormatRevision, profileBinaryFormatRevision, numKeys, numLayers);
    storageHeaderValid =
        logicModelType == 0x01 &&
        configStorageFormatRevision == Kermite_ConfigStorageFormatRevision &&
        profileBinaryFormatRevision == Kermite_ProfileBinaryFormatRevision &&
        numKeys <= 254 &&
        numLayers <= 16;
  }

  if (!storageHeaderValid) {
    kprintf("invalid key assigns data\n");
    // utils_debugShowBytes(storageTempBuf, KeyMappingDataHeaderLength);
  } else {
    kprintf("key assigns storage data is valid\n");
  }

  return storageHeaderValid;
}
