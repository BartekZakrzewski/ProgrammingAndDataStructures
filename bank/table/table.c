#include "table.h"

/* Private functions */
static int max(int a, int b) {
    return a > b ? a : b;
}

static void print_separator(const int *widths, int cols) {
    for (int c = 0; c < cols; c++) {
        putchar('+');
        for (int i = 0; i < widths[c] + 2; i++)
            putchar('-');
    }
    puts("+");
}

static void print_row(const char **cells, const int *widths, int cols) {
    for (int c = 0; c < cols; c++)
        printf("| %-*s ", widths[c], cells[c]);
    puts("|");
}

static char *get_insurance_str(FILE *insurances, const char *accId) {
    char buf[256] = "";
    Insurance ins;

    fseek(insurances, 0, SEEK_SET);
    while (fread(&ins, sizeof(Insurance), 1, insurances) == 1) {
        if (strcmp(ins.accId, accId) == 0) {
            if (buf[0] != '\0')
                strncat(buf, ", ", sizeof(buf) - strlen(buf) - 1);
            strncat(buf, ins.regNumber, sizeof(buf) - strlen(buf) - 1);
        }
    }

    return strdup(buf[0] ? buf : "-");
}

static int validate_search(Account tmp, const char *type, char *query) {
    if (strcmp(type, search[0]) == 0 && strcmp(query, tmp.name) == 0)
        return 1;
    if (strcmp(type, search[1]) == 0 && strcmp(query, tmp.surname) == 0)
        return 1;
    if (strcmp(type, search[2]) == 0 && strcmp(query, tmp.pesel) == 0)
        return 1;
    if (strcmp(type, search[3]) == 0 && strcmp(query, tmp.accId) == 0)
        return 1;
    if (strcmp(type, search[4]) == 0 && strcmp(query, tmp.address) == 0)
        return 1;
    return 0;
}

/* Misc */
int count_records(FILE *accounts, const char *type, char *query) {
    int is_search = (strlen(type) > 0);
    fseek(accounts, 0, SEEK_SET);
    int n = 0;
    Account tmp;
    while (fread(&tmp, sizeof(Account), 1, accounts) == 1) {
        if (is_search) {
            n += validate_search(tmp, type, query);
        } else {
            n++;
        }
    }
    return n;
}

void free_table(Table *t) {
    for (int i = 0; i < t->n_rows * t->n_cols; i++)
        free(t->cells[i]);
    free(t->cells);
}

void compute_col_widths(
    const char **headers, int cols,
    const char **rows, int n_rows,
    int *widths) {
    for (int c = 0; c < cols; c++)
        widths[c] = strlen(headers[c]);

    for (int r = 0; r < n_rows; r++)
        for (int c = 0; c < cols; c++)
            widths[c] = max(widths[c], strlen(rows[r * cols + c]));
}

/* Builders */
Table _build_table(FILE *accounts, FILE *insurances, const char *type, char *query) {
    int is_search = (strlen(type) > 0);
    int n         = count_records(accounts, type, query);
    int cols      = 7;

    char **cells = malloc(n * cols * sizeof(char *));

    Account tmp;
    fseek(accounts, 0, SEEK_SET);

    for (int r = 0; fread(&tmp, sizeof(Account), 1, accounts) == 1; r++) {
        if (is_search && !validate_search(tmp, type, query)) {
            r--;
        } else {
            cells[r * cols + 0] = strdup(tmp.accId);
            cells[r * cols + 1] = strdup(tmp.name);
            cells[r * cols + 2] = strdup(tmp.surname);
            cells[r * cols + 3] = strdup(tmp.address);
            cells[r * cols + 4] = strdup(tmp.pesel);

            char bal[32];
            snprintf(bal, sizeof(bal), "%.2f", tmp.balance);
            cells[r * cols + 5] = strdup(bal);
            cells[r * cols + 6] = get_insurance_str(insurances, tmp.accId);
        }
    }

    return (Table){cells, n, cols};
}

/* Public functions */
void print_table(FILE *accounts, FILE *insurances, const char *type, char *query) {
    Table t = _build_table(accounts, insurances, type, query);

    if (t.n_rows == 0) {
        puts("\nNo records.\n");
        return;
    }

    int widths[t.n_cols];
    compute_col_widths(headers, t.n_cols, (const char **)t.cells, t.n_rows, widths);

    print_separator(widths, t.n_cols);
    print_row(headers, widths, t.n_cols);
    print_separator(widths, t.n_cols);

    for (int r = 0; r < t.n_rows; r++)
        print_row(((const char **)&t.cells[r * t.n_cols]), widths, t.n_cols);

    print_separator(widths, t.n_cols);

    free_table(&t);
}
