#include "../buildCondition.h"
#if defined(KERMITECORE_USE_FLASHSTORAGE_DUMMY_IMPL)

#include "../flashPersistSector.h"
void flashPersistSector_read(uint8_t *bytes4096) {}
bool flashPersistSector_write(uint8_t *bytes4096) {
  return false;
}

#endif
