#include "constants/constants.h"
#include "helpers/helpers.h"
#include "shared/shared.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *accounts = fopen("db/accounts.dat", "r+b");
    if (accounts == NULL) {
        accounts = fopen("db/accounts.dat", "w+b");
    }
    FILE *insurances = fopen("db/insurances.dat", "r+b");
    if (insurances == NULL) {
        insurances = fopen("db/insurances.dat", "w+b");
    }

    int cmd = 0;
    int option = 1;

    while (1) {
        cmd = read_command(&option);
        if (option == MAIN_MENU) {
            if (cmd == QUIT_OR_BACK) {
                break;
            }
            option = cmd;
            cmd = 0;
        }
        if (option == ACCOUNTS || option == INSURANCES) {
            if (cmd == QUIT_OR_BACK) {
                option = MAIN_MENU;
                cmd = 0;
                continue;
            }
            if (cmd == CREATE) {
                create(option == INSURANCES,
                       option == ACCOUNTS ? accounts : insurances);
            } else if (cmd == SEARCH) {
                search(option == INSURANCES,
                       option == ACCOUNTS ? accounts : insurances);
            } else if (cmd == LIST) {
                // printf(option == ACCOUNTS ? "Accounts:\n" : "Insurances:\n");
                list(option == INSURANCES,
                     option == ACCOUNTS ? accounts : insurances);
            }
        }
        if (option == ACCOUNTS && cmd == TRANSFERS) {
            transfer(accounts);
        }
        cmd = 0;
    }

    return 0;
}
