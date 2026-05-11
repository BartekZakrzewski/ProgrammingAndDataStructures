#ifndef SHARED_H
#define SHARED_H

#include "../table/table.h"

#include "../accounts/accounts.h"
#include "../insurances/insurances.h"

#include <time.h>

void create(int option, FILE *fp);
void search(int option, FILE *fp);
void list(int option, FILE *fp);

#endif
