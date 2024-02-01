#if defined(KERMITECORE_USE_FLASHSTORAGE_SAMD_CMAGLIE)

#include "km1/infrastructure/flashPersistSector.h"
#include <FlashStorage.h>

#define DataSize 4096

typedef struct {
  uint8_t buffer[DataSize];
} Bucket;

#define FlashStorageStatic(name, T)                                                                              \
  __attribute__((__aligned__(256))) static const uint8_t PPCAT(_data, name)[(sizeof(T) + 255) / 256 * 256] = {}; \
  static FlashStorageClass<T> name(PPCAT(_data, name));

FlashStorageStatic(bucketStore, Bucket);

void flashPersistSector_read(uint8_t *bytes4096) {
  Bucket bucket = bucketStore.read();
  memcpy(bytes4096, bucket.buffer, DataSize);
}

bool flashPersistSector_write(uint8_t *bytes4096) {
  Bucket bucket;
  memcpy(bucket.buffer, bytes4096, DataSize);
  bucketStore.write(bucket);
  return true;
}

#endif
