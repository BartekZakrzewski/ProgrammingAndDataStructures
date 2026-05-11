#include "../constants/constants.h"
#include "../models.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef HELPERS_H
#define HELPERS_H

void print_banner(int option);
void print_commands(int option);

extern int refresh;
int read_command(int *option);

void read_string(const char *prompt, char *buffer, int max_len);
void generateId(char *id);
int idExists(FILE *fp, char *id);
void generateUniqueId(FILE *fp, char *id);

#endif
