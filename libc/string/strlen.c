#include "string.h"

int strlen(const char *str) {
  const char *start = str;
  while (*str)
    str++;
  return (int)(str - start);
}
