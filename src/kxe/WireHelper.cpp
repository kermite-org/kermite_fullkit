#include "WireHelper.h"

TwoWire *wireHelper_assignWireInstance(int pinSDA, int pinSCL) {
  constexpr uint32_t bitset_pin_sda_i2c0 =
      __bitset({ 0, 4, 8, 12, 16, 20, 24, 28 });
  bool is_pin_sda_i2c0 = (1 << pinSDA) & bitset_pin_sda_i2c0;
  TwoWire *wire = is_pin_sda_i2c0 ? &Wire : &Wire1;
  wire->setSDA(pinSDA);
  wire->setSCL(pinSCL);
  return wire;
}
