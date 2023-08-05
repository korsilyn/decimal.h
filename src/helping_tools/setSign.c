#include "../s21_decimal.h"

void s21_set_sign(s21_decimal *dst) { dst->bits[3] = dst->bits[3] | 1u << 31; }
