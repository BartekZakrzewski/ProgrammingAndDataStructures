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
    }
    FILE *insurances = fopen("db/insurances.dat", "r+b");
    if (insurances == NULL) {
        insurances = fopen("db/insurances.dat", "w+b");
    }

    int option = 0;

    while (1) {
        __refresh();
        print_banner(option == 0);
        if (option == 0) {
            option = action(option, MENU_OPTIONS_LEN);
            if (option == 1)
                break;
        } else if (option == 2) {
            option = action(option, ACCOUNT_OPTIONS_LEN);
            /* Go Back */
            if (option == 1) {
                option = 0;
                continue;
            }
            if (option == 2) {
                /* Create Account */
                try_acc(accounts, insurances);
            } else if (option == 3) {
                /* List Accounts */
                list_acc(accounts, insurances);
            } else if (option == 4) {
                /* Search Accounts */
                search_acc(accounts, insurances);
            } else if (option == 5) {
                /* Widthdraw */
                widthdraw(accounts, insurances);
            } else if (option == 6) {
                /* Deposit */
                deposit(accounts, insurances);
            }
            option = 2;
        } else if (option == 3) {
            /* Create Insurance */
            try_ins(accounts, insurances);
            option = 0;
        } else if (option == 4) {
            /* Transfer Money */
            transfer(accounts, insurances);
            option = 0;
        }
    }

    return 0;
}
