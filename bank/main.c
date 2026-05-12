#include "./actions/actions.h"
#include "./helpers/helpers.h"
#include "./options/options.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    FILE *accounts = fopen("db/accounts.dat", "r+b");
    if (accounts == NULL) {
        accounts = fopen("db/accounts.dat", "w+b");
        if (accounts == NULL) {
            return 1;
        }
    }
    FILE *insurances = fopen("db/insurances.dat", "r+b");
    if (insurances == NULL) {
        insurances = fopen("db/insurances.dat", "w+b");
        if (insurances == NULL) {
            return 1;
        }
    }

    int option = MAIN_MENU;

    while (1) {
        __refresh();
        print_banner(option == MAIN_MENU);
        if (option == MAIN_MENU) {
            option = action(option, MENU_OPTIONS_LEN);
            if (option == EXIT)
                break;
        } else if (option == ACCOUNTS) {
            option = action(option, ACCOUNT_OPTIONS_LEN);
            if (option == EXIT) {
                option = MAIN_MENU;
                continue;
            }

            if (option == ACC_CREATE) {
                try_acc(accounts, insurances);
            } else if (option == ACC_LIST) {
                list_acc(accounts, insurances);
            } else if (option == ACC_SEARCH) {
                search_acc(accounts, insurances);
            } else if (option == ACC_WIDTHDRAW) {
                widthdraw(accounts, insurances);
            } else if (option == ACC_DEPOSIT) {
                deposit(accounts, insurances);
            }
            option = ACCOUNTS;
        } else if (option == INSURANCE) {
            try_ins(accounts, insurances);
            option = MAIN_MENU;
        } else if (option == TRANSFER) {
            transfer(accounts, insurances);
            option = MAIN_MENU;
        }
    }

    fclose(accounts);
    fclose(insurances);

    return 0;
}
