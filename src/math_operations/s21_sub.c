#include "../s21_decimal.h"
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  if (zero(value_1)) {
    *result = value_2;
    if (!zero(value_2)) {
      setBit(result, 127, 1);
    }
  } else if (zero(value_2)) {
    *result = value_1;
  } else {
    if (getBit(value_1, 127) == getBit(value_2, 127)) {  //..
      flag = s21_to_the_same_scale(&value_1, &value_2,
                                   getDegree(value_1) - getDegree(value_2));
      if (!flag) {
        if (s21_is_less(value_1, value_2) && !getBit(value_1, 127)) {
          flag = s21_sub(value_2, value_1, result);
          setBit(result, 127, 1);
        } else if ((s21_is_greater(value_1, value_2) && getBit(value_1, 127))) {
          setBit(&value_2, 127, 0);
          setBit(&value_1, 127, 0);
          flag = s21_sub(value_2, value_1, result);
        } else {
          int fut = 0;
          s21_decimal temp = {{0, 0, 0, 0}};
          for (int i = 0; i < 96; i++) {
            if (getBit(value_1, i) - getBit(value_2, i) - fut < 0) {
              setBit(&temp, i,
                     2 + getBit(value_1, i) - getBit(value_2, i) - fut);
              fut = 1;
            } else {
              setBit(&temp, i, getBit(value_1, i) - getBit(value_2, i) - fut);
              fut = 0;
            }
          }
          *result = temp;
          result->bits[3] = value_1.bits[3];
        }
      }
    } else {
      if (getBit(value_1, 127)) {
        setBit(&value_2, 127, 1);
        flag = s21_add(value_1, value_2, result);
      } else {
        setBit(&value_2, 127, 0);
        flag = s21_add(value_1, value_2, result);
      }
    }
  }
  return flag;
}
