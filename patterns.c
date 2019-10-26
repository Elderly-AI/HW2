/*Условие ИЗ№2:
Сравните и выведите в консоль время работы последовательного и параллельного
с использованием нескольких процессов алгоритмов, каждый из которых выделяет
в динамической памяти символьный массив размером 100 Мб и, рассматривая его
содержимое как абстрактную переписку, определяет эмоциональную окраску
последней. Переписка считается оптимистичной, если диграфов :) в ней больше, чем
диграфов :(; в противном случае переписка признается пессимистичной.
*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "patterns.h"

unsigned int pattern_entity(const char *array, const char *pattern,
                            const size_t pattern_len) {
  if ((array == NULL) || (pattern == NULL)) {
    exit(EINVAL);
  }

  unsigned int pattern_counter = 0;

  const char *line = strstr(array, pattern);
  while(line != NULL){
  	++pattern_counter;
  	line += pattern_len;
  	line = strstr(line, pattern);
  }

  return (pattern_counter);
}

unsigned int pattern_entity_in_slice(const char *f_array, const char *s_array,
                                     const char *pattern,
                                     const size_t pattern_len) {
  if ((f_array == NULL) || (s_array == NULL) || (pattern == NULL)) {
    exit(EINVAL);
  }

  char *buff_array = (char *)malloc(2 * (pattern_len - 1));

  if (buff_array == NULL) {
    exit(ENOMEM);
  }

  size_t f_array_length = strlen(f_array);

  memcpy(buff_array, f_array + f_array_length - pattern_len + 1,
         pattern_len - 1);
  memcpy(buff_array + pattern_len - 1, s_array, pattern_len - 1);

  unsigned int pattern_counter =
      pattern_entity(buff_array, pattern, pattern_len);
  free(buff_array);

  return (pattern_counter);
}

int64_t pattern_entity_in_array(const char **array, const size_t array_size,
                                const char *pattern) {
  if ((array == NULL) || (pattern == NULL)) {
    exit(EINVAL);
  }

  for (size_t c = 0; c < array_size; ++c) {
    if (array[c] == NULL) {
      exit(ENOMEM);
    }
  }

  const size_t pattern_length = strlen(pattern);
  int64_t pattern_counter = 0;

  for (size_t c = 0; c < array_size - 1; ++c) {
    pattern_counter += pattern_entity(array[c], pattern, pattern_length);

    pattern_counter += pattern_entity_in_slice(array[c], array[c + 1], pattern,
                                               pattern_length);
  }
  pattern_counter +=
      pattern_entity(array[array_size - 1], pattern, pattern_length);

  return (pattern_counter);
}
