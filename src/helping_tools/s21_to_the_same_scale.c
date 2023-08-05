#include "../s21_decimal.h"
int s21_to_the_same_scale(s21_decimal *a, s21_decimal *b, int scale_diff) {
  s21_decimal *temp = (s21_decimal *)malloc(sizeof(s21_decimal));
  for (int i = 0; i < 3; i++) temp->bits[i] = 0;
  s21_from_int_to_decimal(pow(10, abs(scale_diff)), temp);
  temp->bits[3] = abs(scale_diff) << 16;
  if (scale_diff < 0)
    s21_mul(*a, *temp, a);
  else
    s21_mul(*b, *temp, b);
  free(temp);
  return 0;
}
