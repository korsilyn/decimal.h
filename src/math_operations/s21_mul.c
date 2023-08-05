#include "../s21_decimal.h"
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, scale = 0;
  s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);
  if (s21_get_sign(value_1) != s21_get_sign(value_2)) s21_set_sign(result);
  scale = s21_get_scale(value_1) + s21_get_scale(value_2);
  error = s21_mul_big_decimal(v1, v2, &r);
  scale = s21_post_normalization(&r, scale);
  if (scale >= 0) {
    s21_set_scale(result, scale);
    s21_import_to_small_decimal(result, r);
  } else {
    error = 1;
  }
  if (error == 1 && s21_get_sign(*result)) error = 2;
  if (error) s21_is_zero(*result);
  return error;
}
