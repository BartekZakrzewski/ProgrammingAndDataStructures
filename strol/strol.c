#include <stdio.h>
#include <stdlib.h>

long int strol(const char *str, char **endptr, int base) {
  long int result = 0;
  const char *ptr = str;

  while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r' || *ptr == '\f' || *ptr == '\v') {
    ptr++;
  }

  int sign = 1;
  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }

  while (*ptr) {
    int digit;
    if (*ptr >= '0' && *ptr <= '9') {
      digit = *ptr - '0';
    } else if (*ptr >= 'a' && *ptr <= 'z') {
      digit = *ptr - 'a' + 10;
    } else if (*ptr >= 'A' && *ptr <= 'Z') {
      digit = *ptr - 'A' + 10;
    } else {
      break;
    }
    if (digit >= base) {
      break;
    }
    result = result * base + digit;
    ptr++;
  }
  if (endptr) {
    *endptr = (char *)ptr;
  }
  return sign * result;
}
