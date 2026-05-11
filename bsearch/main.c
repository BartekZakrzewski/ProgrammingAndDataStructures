#include "func/func.h"
#include <stdio.h>
#include <stdlib.h>

int int_cmp(const void *key, const void *elem) {
    int _key  = *(const int *)key;
    int _elem = *(const int *)elem;
    return (_key > _elem) - (_key < _elem);
}

int double_cmp(const void *key, const void *elem) {
    double _key  = *(const double *)key;
    double _elem = *(const double *)elem;
    return (_key > _elem) - (_key < _elem);
}

void int_tests() {
    printf("========= Int Tests =========\n");
    const int int_base[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int int_key    = 2;
    int *int_res         = bsearch(&int_key, int_base, sizeof(int_base) / sizeof(int_base[0]), sizeof(int), int_cmp);
    if (int_res == NULL) {
        printf("Int res == NULL\n");
    }
    printf("Int bsearch %d\n", *int_res);
}

void double_tests() {
    printf("========= Double Tests ======\n");
    const double double_base[] = {1.1, 2.3, 4, 5.7, 12.3, 15, 127};
    printf("Array: ");
    for (size_t i = 0; i < (sizeof(double_base) / sizeof(double_base[0])); i++) {
        printf("%f, ", double_base[i]);
    }

    const double double_key1 = 5.7;
    printf("\nFor key: %f | Expected: 5.7\n", double_key1);
    double *double_res = bsearch(&double_key1, double_base, sizeof(double_base) / sizeof(double_base[0]), sizeof(double), double_cmp);
    if (double_res == NULL) {
        printf("Double res == NULL\n");
    }
    printf("Double bsearch: %f\n", *double_res);

    const double double_key2 = 128.3;
    printf("\nFor key: %f | Expected: NULL\n", double_key2);
    double_res = bsearch(&double_key2, double_base, sizeof(double_base) / sizeof(double_base[0]), sizeof(double), double_cmp);
    if (double_res == NULL) {
        printf("Double res == NULL\n");
    }
    printf("Double bsearch: %f\n", *double_res);

    const double double_key3 = 5;
    printf("\nFor key: %f | Expected: NULL\n", double_key3);
    double_res = bsearch(&double_key3, double_base, sizeof(double_base) / sizeof(double_base[0]), sizeof(double), double_cmp);
    if (double_res == NULL) {
        printf("Double res == NULL\n");
    }
    printf("Double bsearch: %f\n", *double_res);
}

int main(void) {
    int_tests();
    double_tests();

    return 0;
}
