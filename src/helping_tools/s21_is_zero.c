#include "../s21_decimal.h"
int s21_is_zero(s21_decimal a) {
  int isIt = 1;
  for (int i = 0; i < 3; i++)
    if (a.bits[i] != 0) isIt = 0;
  return isIt;
}
