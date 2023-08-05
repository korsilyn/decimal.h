#include "../s21_decimal.h"

int getDegree(s21_decimal value) {
  unsigned int Mask = 0b11111111 << 16;
  return (value.bits[3] & Mask) >> 16;
}
