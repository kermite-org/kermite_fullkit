#include "firmwareMetadata.h"
#include "../base/utils.h"
#include "kmTypes.h"
#include <string.h>

CommonFirmwareMetadata commonFirmwareMetadata = {
  { '$', 'K', 'M', 'M', 'D' }, //dataHeader
  "000000",                    //projectId
  "00",                        //variationId
  "0000",                      //deviceInstanceCode
  "unnamed keyboard",          //keyboardName
};

CommonFirmwareMetadataSecond commonFirmwareMetadataSecond = {
  "000000", //firmwareId
};

void firmwareMetadata_initialize() {
#ifdef KERMITE_KEYBOARD_NAME
  firmwareMetaData_setKeyboardName(KERMITE_KEYBOARD_NAME);
#endif

#ifdef KERMITE_FIRMWARE_ID
  firmwareMetaData_setFirmwareId(KERMITE_FIRMWARE_ID);
#endif
}

void firmwareMetaData_setKeyboardName(const char *keyboardName) {
  strncpy(commonFirmwareMetadata.keyboardName, keyboardName,
          sizeof(commonFirmwareMetadata.keyboardName));
}

void firmwareMetaData_setFirmwareId(const char *firmwareId) {
  strncpy(commonFirmwareMetadataSecond.firmwareId, firmwareId,
          sizeof(commonFirmwareMetadataSecond.firmwareId));
}
