#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

int validate_base(int base) {
    return (base != 0 && (base < 2 || base > 36));
}

void trim_leading(const char **s, int *neg) {
    while (isspace((unsigned char)**s)) {
        (*s)++;
    }
    if (**s == '-') {
        *neg = 1;
        (*s)++;
    } else if (**s == '+') {
        (*s)++;
    }
}

int detect_base(const char **s, int base) {
    if ((base == 0 || base == 16) && (*s)[0] == '0' && ((*s)[1] == 'x' || (*s)[1] == 'X') && isxdigit((unsigned char)(*s)[2])) {
        (*s) += 2;
        return 16;
    } else if (base == 0) {
        return (**s == '0') ? 8 : 10;
    }
    return base;
}

void calculate_overflow(int neg, long *cutoff, int *cutlim, int base) {
    if (neg) {
        *cutoff = LONG_MIN / base;
        *cutlim = -(int)(LONG_MIN % base);
    } else {
        *cutoff = LONG_MAX / base;
        *cutlim =  (int)(LONG_MAX % base);
    }
}

void iterate_digits(const char **s, int base, int neg, long cutoff, int cutlim, int *any, int *overflow, long *result) {
    for (;; (*s)++) {
        unsigned char c = (unsigned char)**s;
        int digit;

        if (c >= '0' && c <= '9')       digit = c - '0';
        else if (c >= 'a' && c <= 'z')  digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z')  digit = c - 'A' + 10;
        else                            break;

        if (digit >= base) {
            break;
        }

        *any = 1;
        if (*overflow) {
            continue;
        }

        if (neg) {
            if (*result < cutoff || (*result == cutoff && digit > cutlim)) {
                *overflow = 1;
                *result   = LONG_MIN;
            } else {
                *result = *result * base - digit;
            }
        } else {
            if (*result > cutoff || (*result == cutoff && digit > cutlim)) {
                *overflow = 1;
                *result   = LONG_MAX;
            } else {
                *result = *result * base + digit;
            }
        }
    }
}

long int strtol(const char *nptr, char **endptr, int base) {
    const char *s = nptr;
    long result = 0, cutoff;
    int neg = 0, any = 0, overflow = 0, cutlim;

    if (validate_base(base)) {
        errno = EINVAL;
        if (endptr != NULL) {
            *endptr = (char *)nptr;
        }
        return 0;
    }

    trim_leading(&s, &neg);

    base = detect_base(&s, base);

    calculate_overflow(neg, &cutoff, &cutlim, base);

    iterate_digits(&s, base, neg, cutoff, cutlim, &any, &overflow, &result);

    if (overflow) {
        errno = ERANGE;
    }

    if (any < 1) {
      errno = EINVAL;
    }

    if (endptr != NULL) {
        *endptr = (char *)(any ? s : nptr);
    }

    return result;
}
