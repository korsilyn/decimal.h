#include "../s21_decimal.h"
void setBit(s21_decimal *value, int bit, int res) {
  if (getBit(*value, bit) != res) {
    if (getBit(*value, 127)) {
      value->bits[bit / 32] &= ~(1u << (bit % 32));
    } else {
      value->bits[bit / 32] |= 1u << (bit % 32);
    }
  }
}
