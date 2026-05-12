#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include "../constants/constants.h"
#include "../helpers/helpers.h"
#include <stdio.h>

Account read_account(FILE *fp);
void transfer(FILE *fp);

#endif
