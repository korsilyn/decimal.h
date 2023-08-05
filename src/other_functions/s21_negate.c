#include "../s21_decimal.h"
int s21_negate(s21_decimal value, s21_decimal *result) {
  int flag = 0;
  *result = value;
  if (s21_is_zero(value) == 0) {
    if (getBit(value, 127)) {
      result->bits[3] &= ~(1u << 31);
    } else {
      result->bits[3] |= 1u << 31;
    }
  }
  return flag;
}