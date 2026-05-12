#ifndef TABLE_H
#define TABLE_H

#include "../models/models.h"
#include "../options/options.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char **cells;
    int n_rows;
    int n_cols;
} Table;

void print_table(FILE *accounts, FILE *insurances, const char *type, char *query);

#endif
