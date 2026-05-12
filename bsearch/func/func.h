#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *));

#endif
