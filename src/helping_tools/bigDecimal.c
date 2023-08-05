#include "../s21_decimal.h"
void s21_import_to_big_decimal(s21_decimal value_1, s21_big_decimal *value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
}
int s21_from_decimal_to_double(s21_decimal src, long double *dst) {
  long double temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    temp += s21_get_bit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -s21_get_scale(src));
  if (s21_get_bit(src, 127)) temp = temp * (-1);
  *dst = temp;
  return 0;
}

void s21_import_to_small_decimal(s21_decimal *value_1,
                                 s21_big_decimal value_2) {
  value_1->bits[0] = value_2.bits[0];
  value_1->bits[1] = value_2.bits[1];
  value_1->bits[2] = value_2.bits[2];
}

void s21_set_bit_big(s21_big_decimal *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}

int s21_get_bit_big(s21_big_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

void s21_shift_big_dec_right(s21_big_decimal *dst, int num) {
  int buffer[7] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_big(*dst, (i + 1) * 32);
    }
    for (int i = 0; i < 7; i++) {
      dst->bits[i] >>= 1;
      s21_set_bit_big(dst, (i + 1) * 32 - 1, buffer[i]);
    }
    dst->bits[7] >>= 1;
  }
}

// Сдвиг big decimal влево +
int s21_shift_big_dec_left(s21_big_decimal *dst, int num) {
  int error = 0;
  int buffer[8] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_big(*dst, (i + 1) * 32 - 1);
    }
    for (int i = 7; i > 0 && !error; i--) {
      if (s21_get_bit_big(*dst, 255)) error = 1;
      dst->bits[i] <<= 1;
      s21_set_bit_big(dst, i * 32, buffer[i - 1]);
    }
    dst->bits[0] <<= 1;
  }
  return error;
}

void s21_normalization(s21_big_decimal *value_1, s21_big_decimal *value_2,
                       int diff) {
  if (diff > 0) {
    s21_increase_scale_big_decimal(value_2, diff);
  } else if (diff < 0) {
    s21_increase_scale_big_decimal(value_1, -diff);
  }
}

int s21_post_normalization(s21_big_decimal *result, int scale) {
  int dop = 0;
  while ((result->bits[3] || result->bits[4] || result->bits[5] ||
          result->bits[6] || result->bits[7]) &&
         scale > 0) {
    if (scale == 1 && result->bits[3]) dop = 1;
    s21_decreace_scale_big_decimal(result, 1);
    scale--;
  }
  if (dop && scale == 0 && result->bits[3] == 0 && s21_get_bit_big(*result, 0))
    s21_set_bit_big(result, 0, 0);
  if ((result->bits[3] || result->bits[4] || result->bits[5] ||
       result->bits[6] || result->bits[7]))
    scale = -1;
  return scale;
}

void s21_zero_big_decimal(s21_big_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[4] =
      dst->bits[5] = dst->bits[6] = dst->bits[7] = 0;
}

void s21_increase_scale_big_decimal(s21_big_decimal *dst, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_mul_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

int s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {
  int error = 0, count = 0;
  for (int i = 0; i < 256 && !error; i++) {
    if (s21_get_bit_big(value_2, i)) {
      error = s21_shift_big_dec_left(&value_1, i - count);
      s21_add_big_decimal(value_1, *result, result);
      count = i;
    }
  }
  return error;
}

void s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {
  int res = 0, ovf = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) + s21_get_bit_big(value_2, i) + ovf;
    ovf = res / 2;
    s21_set_bit_big(result, i, res % 2);
  }
}
void s21_decreace_scale_big_decimal(s21_big_decimal *dst, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_div_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

int s21_get_integer_part(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result) {
  int high_bit = 0, b1 = 0, b2 = 0, error = 0;
  s21_big_decimal tmp_rem = {0}, v1 = {0}, v2 = {0}, nvalue = {0},
                  one = {{1, 0, 0, 0, 0, 0, 0, 0}};
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);
  while (s21_is_greater_big_decimal(v2, v1)) {
    s21_increase_scale_big_decimal(&v1, 1);
    s21_set_scale(&value_1, s21_get_scale(value_1) + 1);
  }
  int scale = s21_get_scale(value_2) - s21_get_scale(value_1);
  s21_find_highest_bit_big_decimal(v1, v2, &b1, &b2);
  for (int j = b1; j >= 0; j--) {
    s21_set_bit_big(&nvalue, j, !s21_get_bit_big(v2, j));
  }
  s21_add_big_decimal(nvalue, one, &nvalue);
  for (int j = b1; j >= 0; j--) {
    s21_shift_big_dec_left(&tmp_rem, 1);
    high_bit = s21_get_bit_big(tmp_rem, b1);
    s21_set_bit_big(&tmp_rem, b1 + 1, 0);
    s21_set_bit_big(&tmp_rem, 0, s21_get_bit_big(v1, b1));
    s21_shift_big_dec_left(&v1, 1);
    s21_set_bit_big(&v1, b1 + 1, 0);
    if (high_bit) {
      s21_add_big_decimal(tmp_rem, v2, &tmp_rem);
    } else {
      s21_add_big_decimal(tmp_rem, nvalue, &tmp_rem);
    }
    s21_set_bit_big(&tmp_rem, b1 + 1, 0);
    high_bit = s21_get_bit_big(tmp_rem, b1);
    s21_set_bit_big(&v1, 0, !high_bit);
  }
  if (scale < 0) {
    s21_increase_scale_big_decimal(&v1, -scale);
    scale = s21_post_normalization(&v1, 0);
  } else {
    scale = s21_post_normalization(&v1, scale);
  }
  if (scale >= 0) {
    s21_set_scale(result, scale);
    s21_import_to_small_decimal(result, v1);
  } else {
    error = 1;
  }
  if (error == 1 && s21_get_sign(*result)) error = 2;
  if (error) s21_is_zero(*result);
  return error;
}

void s21_find_highest_bit_decimal(s21_decimal v1, s21_decimal v2, int *bit_1,
                                  int *bit_2) {
  for (int i = 95; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_get_bit(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_get_bit(v2, i)) *bit_2 = i;
  }
}
int s21_is_greater_big_decimal(s21_big_decimal value_1,
                               s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) out = 1;
    }
  }
  return result;
}

int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !out && !result; i--) {
    if (value_1.bits[i] != 0 || value_2.bits[i] != 0) {
      if (value_1.bits[i] >= value_2.bits[i]) {
        result = 1;
      }
      out = 1;
    }
  }
  return result;
}

int s21_is_decimal_no_empty(s21_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2];
}

int s21_is_big_decimal_not_empty(s21_big_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2] + dst.bits[3] + dst.bits[4] +
         dst.bits[5] + dst.bits[6] + dst.bits[7];
}

void s21_find_highest_bit_big_decimal(s21_big_decimal v1, s21_big_decimal v2,
                                      int *bit_1, int *bit_2) {
  for (int i = 255; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_get_bit_big(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_get_bit_big(v2, i)) *bit_2 = i;
  }
}

int s21_equation_bits_big_decimal(s21_big_decimal *value_1,
                                  s21_big_decimal *value_2) {
  int scale = 0;
  while (s21_is_greater_big_decimal(*value_2, *value_1)) {
    s21_increase_scale_big_decimal(value_1, 1);
    scale++;
  }
  while (s21_is_greater_or_equal_big_decimal(*value_1, *value_2)) {
    s21_shift_big_dec_left(value_2, 1);
  }
  s21_shift_big_dec_right(value_2, 1);
  return scale;
}
