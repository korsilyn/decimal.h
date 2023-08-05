#include "../s21_decimal.h"
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (s21_is_decimal_no_empty(value_2)) {
    int scale = 0, res_scale = 0;
    s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);
    if (s21_get_sign(value_1) != s21_get_sign(value_2)) s21_set_sign(result);
    scale = s21_div_big_decimal(v1, v2, &r);
    s21_set_scale(&value_1, s21_get_scale(value_1) + scale);
    res_scale = s21_get_scale(value_1) - s21_get_scale(value_2);
    if (res_scale > 0) {
      res_scale = s21_post_normalization(&r, res_scale);
    } else if (res_scale < 0) {
      s21_increase_scale_big_decimal(&r, abs(res_scale));
      res_scale = s21_post_normalization(&r, 0);
    }
    if (res_scale >= 0) {
      s21_import_to_small_decimal(result, r);
      s21_set_scale(result, res_scale);
    } else {
      error = 1;
    }
  } else {
    error = 3;
  }
  if (error == 1 && s21_get_sign(*result)) error = 2;
  if (error) s21_zero_decimal(result);
  return error;
}

int s21_div_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, scale = 0, diff = 0, save_scale = 0;
  s21_big_decimal tmp = {0};
  s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;
  for (int i = 0; i < 96 && s21_is_big_decimal_not_empty(value_1);) {
    if (i > 0) {
      s21_shift_big_dec_left(&value_2, 1);
      s21_increase_scale_big_decimal(result, 1);
      s21_increase_scale_big_decimal(&value_1, 1);
      save_scale++;
    }
    scale = s21_equation_bits_big_decimal(&value_1, &value_2);
    save_scale += scale;
    b_1 = b_2 = 0;
    s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    if (diff < 0) diff = 0;
    for (; diff >= 0 && s21_is_big_decimal_not_empty(value_1);) {
      if (s21_is_greater_big_decimal(value_2, value_1)) {
        s21_set_bit_big(&tmp, 0, 0);
      } else {
        s21_sub_big_decimal(value_1, value_2, &value_1);
        s21_set_bit_big(&tmp, 0, 1);
      }
      i++;
      diff--;
      if (diff >= 0) s21_shift_big_dec_right(&value_2, 1);
      s21_shift_big_dec_left(&tmp, 1);
    }
    if (diff >= 0) s21_shift_big_dec_left(&tmp, diff + 1);
    s21_shift_big_dec_right(&tmp, 1);
    s21_add_big_decimal(*result, tmp, result);
    s21_zero_big_decimal(&tmp);
  }
  return save_scale;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign = 0;
  if (s21_get_sign(value_1)) sign = 1;
  s21_set_bit(&value_1, 127, 0);
  s21_set_bit(&value_2, 127, 0);
  if (!s21_is_decimal_no_empty(value_2)) {
    error = 3;
  } else {
    if (s21_is_greater_or_equal(value_1, value_2)) {
      s21_decimal tmp = {0};
      error = s21_get_integer_part(value_1, value_2, &tmp);
      if (!error) {
        s21_decimal tmp1 = {0};
        error = s21_mul(value_2, tmp, &tmp1);
        if (!error) {
          error = s21_sub(value_1, tmp1, result);
        }
      }
    } else {
      *result = value_1;
    }
  }

  s21_set_bit(result, 127, sign);
  if (error == 1 && s21_get_sign(*result)) error = 2;
  if (error) s21_is_zero(*result);
  return error;
}
void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {
  int tmp = 0, res = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) - s21_get_bit_big(value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_bit_big(result, i, res % 2);
  }
}
