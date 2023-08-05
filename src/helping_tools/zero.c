#include "../s21_decimal.h"
int zero(s21_decimal a) {
  int flag = 1;
  for (int i = 0; i < 3; i++)
    if (a.bits[i] != 0) flag = 0;
  return flag;
}
