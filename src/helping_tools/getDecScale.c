#include "../s21_decimal.h"
int getDecScale(s21_decimal num) {
  int powD = 0;
  if (s21_get_sign(num) == 1) {
    num.bits[3] = num.bits[3] >> 2;
    num.bits[3] = num.bits[3] - pow(2, 29);
    num.bits[3] = num.bits[3] << 2;
    powD = num.bits[3] >> 16;
    if (powD < 0) powD = abs(powD);
  } else {
    powD = num.bits[3] >> 16;
  }
  return powD;
}