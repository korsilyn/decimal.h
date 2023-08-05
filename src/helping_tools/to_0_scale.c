#include "../s21_decimal.h"
int to_0_scale(s21_decimal *a) {
  int flag = 1;
  s21_decimal *temp = (s21_decimal *)malloc(sizeof(s21_decimal));
  for (int i = 0; i < 4; i++) temp->bits[i] = 0;
  s21_from_int_to_decimal(10, temp);
  int scale = getDegree(*a);
  a->bits[3] = (getBit(*a, 127) << 31) | 0;
  for (int i = 0; i < scale; i++) {
    if (!s21_div(*a, *temp, a)) {
      flag = 0;
    }
  }
  free(temp);
  return flag;
}
