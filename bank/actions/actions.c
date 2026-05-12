#include "actions.h"
#include <stdio.h>

/* Insurance actions */
int try_ins(FILE *accounts, FILE *insurances) {
    __refresh();
    print_banner(0);
    Insurance insurance = _create_insurance(insurances);

    int is_valid_ins = validate_insurance(insurance, accounts);
    if (is_valid_ins == -1)
        printf("User not found\n");
    else if (is_valid_ins == -2)
        printf("Invalid  Car Registration Number\n");
    else if (is_valid_ins == -3)
        printf("Price has to be number and cannot be negative\n");
    if (is_valid_ins < 0) {
        sleep(2);
        return -1;
    }

    FILE *_insurances = fopen("db/insurances_buffer.dat", "w+b");
    if (_insurances == NULL) {
        return -4;
    }
    fseek(_insurances, 0, SEEK_SET);
    fwrite(&insurance, sizeof(Insurance), 1, _insurances);
    print_table(accounts, _insurances, "ID", insurance.accId);
    fclose(_insurances);
    _insurances = NULL;
    remove("db/insurances_buffer.dat");
    printf("\nConfirm creating insurance? (y/n): ");

    char confirm[4];
    read_string("", confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("Cancelled.\n");
        sleep(1);
        return 1;
    }
    fseek(insurances, 0, SEEK_END);
    printf("Creating insurance...\n");
    sleep(1);
    fwrite(&insurance, sizeof(Insurance), 1, insurances);
    printf("Insurance created successfully.\n");
    sleep(1);

    return 0;
}

/* Account actions */
int try_acc(FILE *fp, FILE *__ins) {
    __refresh();
    print_banner(0);
    Account account = _create_account(fp);

    int is_valid_acc = validate_account(account);
    if (is_valid_acc == -1)
        printf("Invalid Name or Surname\n");
    else if (is_valid_acc == -2)
        printf("Invalid PESEL\n");
    else if (is_valid_acc == -3)
        printf("Invalid balance\n");
    if (is_valid_acc < 0) {
        sleep(2);
        return -1;
    }

    FILE *_accounts = fopen("db/accounts_buffer.dat", "w+b");
    if (_accounts == NULL) {
        return -4;
    }
    fseek(_accounts, 0, SEEK_SET);
    fwrite(&account, sizeof(Account), 1, _accounts);
    print_table(_accounts, __ins, "ID", account.accId);
    fclose(_accounts);
    _accounts = NULL;
    remove("db/accounts_buffer.dat");
    printf("\nConfirm creating account? (y/n): ");

    char confirm[4];
    read_string("", confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("Cancelled.\n");
        return 1;
    }
    printf("Creating account...\n");
    sleep(1);
    fseek(fp, 0, SEEK_END);
    fwrite(&account, sizeof(Account), 1, fp);
    printf("Account created successfully.\n");
    sleep(1);

    return 0;
}

void list_acc(FILE *accounts, FILE *insurances) {
    print_table(accounts, insurances, "", "");
    __exit();
}

void search_acc(FILE *accounts, FILE *insurances) {
    __refresh();
    print_banner(0);
    int _search;
    printf("Coose search type (1 - %d)\n", n_search);
    for (int opt = 0; opt < n_search; opt++) {
        printf("%d. %s\n", opt + 1, search[opt]);
    }
    printf("\nOption: ");
    scanf("%d", &_search);
    char c;
    while ((c = getchar()) != '\n') {
    }
    while (_search < 1 || _search > n_search) {
        printf("Choose option between 1 and %d\n\nOption: ", n_search);
        scanf("%d", &_search);
    }

    char buffer[64];
    printf("Enter %s: ", search[_search - 1]);
    read_string("", buffer, sizeof(buffer));

    print_table(accounts, insurances, search[_search - 1], buffer);
    __exit();
}

/* Transfers */
void transfer(FILE *accounts, FILE *insurances) {
    __refresh();
    print_banner(0);

    char __from[64];
    read_string("Enter ID of account sending transfer: ", __from, sizeof(__from));

    char __to[64];
    read_string("Enter ID of account recieving transfer: ", __to, sizeof(__to));

    char __am[64];
    read_string("Enter amount to transfer: ", __am, sizeof(__am));
    double __amount = atof(__am);
    while (__amount < 0) {
        printf("Amount has to be >0: ");
        read_string("Amount has to be >0: ", __am, sizeof(__am));
        __amount = atof(__am);
    }
    printf("\n\nAmount: %.2f\n\n", __amount);

    int valid_from = 0;
    Account from;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&from, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(__from, from.accId) == 0) {
            valid_from = 1;
            break;
        }
    }

    int valid_to = 0;
    Account to;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&to, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(__to, to.accId) == 0) {
            valid_to = 1;
            break;
        }
    }

    if (!valid_to || !valid_from) {
        printf("Invalid account id\n");
        sleep(1);
        return;
    }

    if (strcmp(__from, __to) == 0) {
        printf("Cannot transfer to the same account!\n");
        sleep(1);
        return;
    }

    if (__amount > from.balance) {
        printf("Not enough money\n");
        sleep(1);
        return;
    }

    from.balance -= __amount;
    to.balance += __amount;

    printf("\n\nAccount sending transfer after transaction\n\n");
    FILE *_accounts = fopen("db/accounts_buffer.dat", "w+b");
    if (_accounts == NULL) {
        return;
    }
    fseek(_accounts, 0, SEEK_SET);
    fwrite(&from, sizeof(Account), 1, _accounts);
    print_table(_accounts, insurances, "ID", from.accId);
    fclose(_accounts);
    _accounts = NULL;
    remove("db/accounts_buffer.dat");

    printf("\n\nAccount receiving transfer after transaction\n\n");
    _accounts = fopen("db/accounts_buffer.dat", "w+b");
    if (_accounts == NULL) {
        return;
    }
    fseek(_accounts, 0, SEEK_SET);
    fwrite(&to, sizeof(Account), 1, _accounts);
    print_table(_accounts, insurances, "ID", to.accId);
    fclose(_accounts);
    _accounts = NULL;
    remove("db/accounts_buffer.dat");

    printf("\nConfirm? (y/n): ");

    char confirm[4];
    read_string("", confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("Cancelled.\n");
        sleep(1);
        return;
    }

    printf("Transfering...\n");
    sleep(1);
    Account tmp;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&tmp, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(to.accId, tmp.accId) == 0) {
            fseek(accounts, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&to, sizeof(Account), 1, accounts);
            fflush(accounts);
        } else if (strcmp(from.accId, tmp.accId) == 0) {
            fseek(accounts, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&from, sizeof(Account), 1, accounts);
            fflush(accounts);
        }
    }
    printf("Transfer successful.\n");
    sleep(1);
}

void widthdraw(FILE *accounts, FILE *insurances) {
    __refresh();
    print_banner(0);

    char __from[64];
    read_string("Enter ID of widthdrawing account: ", __from, sizeof(__from));

    char __am[64];
    read_string("Enter amount to widthdraw: ", __am, sizeof(__am));
    double __amount = atof(__am);
    while (__amount < 0) {
        printf("Amount has to be >0: ");
        read_string("Amount has to be >0: ", __am, sizeof(__am));
        __amount = atof(__am);
    }
    printf("\n\nAmount: %.2f\n\n", __amount);

    int valid_from = 0;
    Account from;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&from, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(__from, from.accId) == 0) {
            valid_from = 1;
            break;
        }
    }

    if (!valid_from) {
        printf("Invalid account id\n");
        sleep(1);
        return;
    }

    if (__amount > from.balance) {
        printf("Not enough money\n");
        sleep(2);
        return;
    }

    from.balance -= __amount;

    printf("\n\nAccount after widthdrawing\n\n");
    FILE *_accounts = fopen("db/accounts_buffer.dat", "w+b");
    if (_accounts == NULL) {
        return;
    }
    fseek(_accounts, 0, SEEK_SET);
    fwrite(&from, sizeof(Account), 1, _accounts);
    print_table(_accounts, insurances, "ID", from.accId);
    fclose(_accounts);
    _accounts = NULL;
    remove("db/accounts_buffer.dat");

    printf("\nConfirm? (y/n): ");

    char confirm[4];
    read_string("", confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("Cancelled.\n");
        sleep(2);
        return;
    }

    printf("Widthdrawing...\n");
    sleep(1);
    Account tmp;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&tmp, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(from.accId, tmp.accId) == 0) {
            fseek(accounts, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&from, sizeof(Account), 1, accounts);
            fflush(accounts);
        }
    }
    printf("Widthdrawal successful.\n");
    sleep(1);
}

void deposit(FILE *accounts, FILE *insurances) {
    __refresh();
    print_banner(0);

    char __from[64];
    read_string("Enter ID of depositing account: ", __from, sizeof(__from));

    char __am[64];
    read_string("Enter amount to deposit: ", __am, sizeof(__am));
    double __amount = atof(__am);
    printf("\n\nAmount: %.2f\n\n", __amount);

    int valid_from = 0;
    Account from;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&from, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(__from, from.accId) == 0)
            break;
    }

    if (!valid_from) {
        printf("Invalid account id\n");
        sleep(1);
        return;
    }

    from.balance += __amount;

    printf("\n\nAccount after deposit\n\n");
    FILE *_accounts = fopen("db/accounts_buffer.dat", "w+b");
    if (_accounts == NULL) {
        return;
    }
    fseek(_accounts, 0, SEEK_SET);
    fwrite(&from, sizeof(Account), 1, _accounts);
    print_table(_accounts, insurances, "ID", from.accId);
    fclose(_accounts);
    _accounts = NULL;
    remove("db/accounts_buffer.dat");

    printf("\nConfirm? (y/n): ");

    char confirm[4];
    read_string("", confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("Cancelled.\n");
        sleep(2);
        return;
    }

    printf("Depositing...\n");
    sleep(1);
    Account tmp;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&tmp, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(from.accId, tmp.accId) == 0) {
            fseek(accounts, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&from, sizeof(Account), 1, accounts);
            fflush(accounts);
        }
    }
    printf("Deposit successful.\n");
    sleep(1);
}
