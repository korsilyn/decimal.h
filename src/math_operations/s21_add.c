#include "../s21_decimal.h"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  if (zero(value_1)) {
    *result = value_2;
  } else if (zero(value_2)) {
    *result = value_1;
  } else {
    if (getBit(value_1, 127) == getBit(value_2, 127)) {
      flag = s21_to_the_same_scale(&value_1, &value_2,
                                   getDegree(value_1) - getDegree(value_2));
      if (!flag) {
        unsigned int past = 0;
        s21_decimal temp = {{0, 0, 0, 0}};
        for (int i = 0; i < 96; i++) {
          setBit(&temp, i,
                 (past + getBit(value_1, i) + getBit(value_2, i)) % 2);
          past = (getBit(value_1, i) + getBit(value_2, i) + past) / 2;
        }
        if (past) {
          if (!getBit(value_1, 127)) {
            flag = 1;
          } else {
            flag = 2;
          }
        } else {
          *result = temp;
          result->bits[3] = value_1.bits[3];
        }
      }
    } else {
      if (getBit(value_1, 127)) {
        setBit(&value_1, 127, 0);
        flag = s21_sub(value_2, value_1, result);
      } else {
        setBit(&value_2, 127, 0);
        flag = s21_sub(value_1, value_2, result);
      }
    }
  }
  return flag;
}
