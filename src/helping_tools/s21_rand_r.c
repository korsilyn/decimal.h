#include "../s21_decimal.h"

// генератор случайных чисел
float s21_rand_r(float a, float b) {
  float m = (float)rand() / RAND_MAX;
  float num = a + m * (b - a);
  return num;
}