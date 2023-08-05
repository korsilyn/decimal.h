#include "../s21_decimal.h"
// Изменение флага на противоложный
void s21_comparison_reverse(int *flag) {
  if (*flag == 1) {
    *flag = 2;
  } else if (*flag == 2) {
    *flag = 1;
  }
}

// Побитовое сравнение двух decimal
int s21_comparison_bits(s21_decimal d1, s21_decimal d2) {
  int bit1 = 0;
  int bit2 = 0;
  int flag = 0;
  for (int i = 95; i >= 0 && !flag; --i) {
    bit1 = s21_get_bit(d1, i);
    bit2 = s21_get_bit(d2, i);
    if (bit1 > bit2) {
      flag = 1;
    } else if (bit1 < bit2) {
      flag = 2;
    }
  }
  return flag;
}

// Поиск наибольшего числа
// Возвращает 0, 1, 2, если a = b, a > b, a < b соответсвенно
int s21_greater_num(int a, int b) {
  int flag = 0;
  if (a > b) {
    flag = 1;
  } else if (a < b) {
    flag = 2;
  } else if (a == b) {
    flag = 0;
  }
  return flag;
}

// Преобразоветли
// Возвращает 0, 1, 2, если d1 = d2, d1 > d2, d1 < d2 соответсвенно
int s21_comparison(s21_decimal d1, s21_decimal d2) {
  int flag = 0;
  int scale_dif = (s21_get_scale(d1) - s21_get_scale(d2));
  s21_normalize(&d1, &d2);
  flag = s21_greater_num(!s21_get_sign(d1), !s21_get_sign(d2));
  if (flag == 0) {
    if (scale_dif >= 0) {
      flag = s21_comparison_bits(d1, d2);
    } else {
      scale_dif = -scale_dif;
      flag = s21_comparison_bits(d2, d1);
      s21_comparison_reverse(&flag);
    }
    if (s21_get_sign(d1) && s21_get_sign(d2)) {
      s21_comparison_reverse(&flag);
    }
  } else {
    if (d1.bits[0] == 0 && d1.bits[1] == 0 && d1.bits[2] == 0 &&
        d2.bits[0] == 0 && d2.bits[1] == 0 && d2.bits[2] == 0) {
      flag = 0;
    }
  }
  return flag;
}

// Функция Меньше <
int s21_is_less(s21_decimal d1, s21_decimal d2) {
  return (s21_comparison(d1, d2) == 2) ? 1 : 0;
}

// Функция Меньше или равно <=
int s21_is_less_or_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_comparison(d1, d2) == 2) || (s21_comparison(d1, d2) == 0) ? 1 : 0;
}

// Функция Больше >
int s21_is_greater(s21_decimal d1, s21_decimal d2) {
  return (s21_comparison(d1, d2) == 1) ? 1 : 0;
}

// Функция Больше или равно >=
int s21_is_greater_or_equal(s21_decimal d1, s21_decimal d2) {
  return (s21_comparison(d1, d2) == 1) || (s21_comparison(d1, d2) == 0) ? 1 : 0;
}
// Копирование d2 в d1
void s21_copy_decimal(s21_decimal *d1, s21_decimal d2) {
  for (int i = 127; i >= 0; i--) {
    s21_set_bit(d1, i, s21_get_bit(d2, i));
  }
}

// Повышение scale на norm
void s21_normalize_scale_upper(s21_decimal *d, int norm) {
  s21_decimal _norm = {0};
  s21_decimal _res = {0};
  s21_from_float_to_decimal(pow(10, norm), &_norm);
  s21_mul(*d, _norm, &_res);
  s21_set_scale(&_res, s21_get_scale(*d) + norm);
  s21_copy_decimal(d, _res);
}

// Нормализация d1 d2 путем уравнения их scale
void s21_normalize(s21_decimal *d1, s21_decimal *d2) {
  int norm = s21_get_scale(*d1) - s21_get_scale(*d2);
  if (norm > 0) {
    s21_normalize_scale_upper(d2, norm);
  } else if (norm < 0) {
    s21_normalize_scale_upper(d1, -norm);
  }
}