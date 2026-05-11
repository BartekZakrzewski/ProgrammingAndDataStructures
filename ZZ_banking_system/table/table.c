#include "table.h"
#include <stdio.h>
#include <string.h>

void horizontal_border(const int *n_headers, const int *cell_width) {
    for (int i = 0; i < *n_headers; i++) {
        if (i == 0)
            printf("+");
        for (int j = 1; j < *cell_width - 1; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");
}

void inner_cell(const char **content, const int *n_content,
                const int *cell_width) {
    for (int i = 0; i < *n_content; i++) {
        printf("|");
        int padding = (*cell_width - 2 - strlen(content[i])) / 2;
        for (int j = 0; j < padding; j++) {
            printf(" ");
        }
        printf("%s", content[i]);
        for (int j = 0; j < *cell_width - 2 - padding - strlen(content[i]);
             j++) {
            printf(" ");
        }
    }
    printf("|\n");
}

void table_top(const char **headers, const int *n_headers,
               const int *cell_width) {
    horizontal_border(n_headers, cell_width);
    inner_cell(headers, n_headers, cell_width);
    horizontal_border(n_headers, cell_width);
}

void content_cell(const char **content, const int *n_content,
                  const int *cell_width) {
    inner_cell(content, n_content, cell_width);
    horizontal_border(n_content, cell_width);
}
