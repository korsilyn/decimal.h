#include "../s21_decimal.h"
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_zero_decimal(dst);
  int error = 0;
  if (src < 0) {
    s21_set_sign(dst);
    src = (-1) * src;
  }
  if (src > INT_MAX)
    error = 1;
  else
    dst->bits[0] = src;
  return error;
}
