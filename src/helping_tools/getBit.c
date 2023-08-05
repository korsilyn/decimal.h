#include "../s21_decimal.h"
int getBit(s21_decimal value, int bit) {
  int mask = 1u << (bit % 32);
  return (value.bits[bit / 32] & mask) != 0;
}
