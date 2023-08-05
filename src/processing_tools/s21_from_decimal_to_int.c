#include "../s21_decimal.h"
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  int scale = getDegree(src);
  if (src.bits[1] || src.bits[2]) {
    error = 1;
  } else {
    *dst = src.bits[0];
    if (scale > 0 && scale <= 28) {
      *dst /= pow(10, scale);
    }
  }
  if (getBit(src, 127)) *dst = *dst * (-1);
  return error;
}
