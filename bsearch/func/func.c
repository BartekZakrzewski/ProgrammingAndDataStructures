#include "func.h"

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *)) {
    const unsigned char *element = (const unsigned char *)base;
    size_t left                  = 0;
    size_t right                 = num;

    while (left < right) {
        size_t mid              = left + (right - left) / 2;
        const void *mid_element = element + mid * size;
        int comp                = compar(key, mid_element);
        if (comp == 0) {
            return (void *)mid_element;
        } else if (comp < 0) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return NULL;
}
