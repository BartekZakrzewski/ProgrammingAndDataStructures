#ifndef TABLE_H
#define TABLE_H

void horizontal_border(const int *n_headers, const int *cell_width);
void inner_cell(const char **content, const int *n_content,
                const int *cell_width);

void table_top(const char **headers, const int *n_headers,
               const int *cell_width);
void content_cell(const char **content, const int *n_content,
                  const int *cell_width);

#endif
