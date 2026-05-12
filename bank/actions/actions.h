#ifndef ACTIONS_H
#define ACTIONS_H

#include "../helpers/helpers.h"
#include "../models/models.h"
#include "../options/options.h"
#include "../table/table.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Menu layers definitions */
#define MAIN_MENU 0
#define EXIT 1
#define ACCOUNTS 2
#define INSURANCE 3
#define TRANSFER 4

#define ACC_CREATE 2
#define ACC_LIST 3
#define ACC_SEARCH 4
#define ACC_WIDTHDRAW 5
#define ACC_DEPOSIT 6

/* Insurance actions */
int try_ins(FILE *accounts, FILE *insurances);

/* Account actions */
int try_acc(FILE *fp, FILE *__ins);
void list_acc(FILE *accounts, FILE *insurances);
void search_acc(FILE *accounts, FILE *insurances);

/* Transfers */
void transfer(FILE *accounts, FILE *insurances);
void widthdraw(FILE *accounts, FILE *insurances);
void deposit(FILE *accounts, FILE *insurances);

#endif
