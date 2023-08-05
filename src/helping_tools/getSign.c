#include "../s21_decimal.h"
int s21_get_sign(s21_decimal dst) { return (dst.bits[3] & 1u << 31) != 0; }
