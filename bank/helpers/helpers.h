#ifndef HELPERS_H
#define HELPERS_H

#include "../models/models.h"
#include "../options/options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Helpers */
void read_string(const char *prompt, char *buffer, int max_len);
void generateUniqueId(FILE *fp, char *id);
extern int refresh;
void __refresh();
void print_banner(int show_welcome);
int action(int option, int options_len);

/* Validators */
// static int validate_letters(char *str);
int validate_account(Account account);
int validate_insurance(Insurance insurance, FILE *accounts);

/* Misc */
Insurance _create_insurance(FILE *insurances);
Account _create_account(FILE *fp);
void __exit();

#endif
