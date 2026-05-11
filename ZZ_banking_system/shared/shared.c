#include "shared.h"

void create(int option, FILE *fp) {
    if (!option) {
        Account account = read_account(fp);
        printf("\nConfirm creating account for %s %s, %s %s %f? (y/n): ",
               account.name, account.surname, account.address, account.pesel,
               account.balance);
        char confirm[4];
        read_string("", confirm, sizeof(confirm));
        if (confirm[0] != 'y' && confirm[0] != 'Y') {
            printf("Cancelled.\n");
            return;
        }
        fseek(fp, 0, SEEK_END);
        fwrite(&account, sizeof(Account), 1, fp);
        printf("Account created successfully.\n");
    } else {
        Insurance insurance = read_insurance(fp);
        printf("\nConfirm creating insurance for account %s, car %s, price %f? "
               "(y/n): ",
               insurance.accId, insurance.regNumber, insurance.price);
        char confirm[4];
        read_string("", confirm, sizeof(confirm));
        if (confirm[0] != 'y' && confirm[0] != 'Y') {
            printf("Cancelled.\n");
            return;
        }
        fseek(fp, 0, SEEK_END);
        fwrite(&insurance, sizeof(Insurance), 1, fp);
        printf("Insurance created successfully.\n");
    }
}

void search(int option, FILE *fp) {
    if (!option) {
        printf("Search account by:\n");
        printf("1. ID\n");
        printf("2. Name\n");
        printf("3. Surname\n");
        printf("4. Address\n");
        printf("5. PESEL\n");
        printf("Choose option: ");
        char c;
        scanf("%c", &c);
        while (getchar() != '\n') {
        }
        if (c < '1' || c > '5') {
            printf("Invalid option. Please try again.\n");
            return search(option, fp);
        }
        char search_term[100];
        printf("Enter search term: ");
        fgets(search_term, sizeof(search_term), stdin);
        search_term[strcspn(search_term, "\n")] = '\0';

        Account account;
        table_top(headers[0], &headers_count[0], &cell_width[0]);
        while (fread(&account, sizeof(Account), 1, fp) == 1) {
            if ((c == '1' && strcmp(account.accId, search_term) == 0) ||
                (c == '2' && strcmp(account.name, search_term) == 0) ||
                (c == '3' && strcmp(account.surname, search_term) == 0) ||
                (c == '4' && strcmp(account.address, search_term) == 0) ||
                (c == '5' && strcmp(account.pesel, search_term) == 0)) {
                char balance[50];
                snprintf(balance, 50, "%.2f", account.balance);
                const char *content[] = {account.accId,   account.name,
                                         account.surname, account.address,
                                         account.pesel,   balance};
                content_cell(content, &headers_count[0], &cell_width[0]);
            }
        }
        char e;
        while (1) {
            printf("Exit? (y/Y): ");
            scanf("%c", &e);
            while (getchar() != '\n') {
            }
            if (e == 'y' || e == 'Y')
                break;
        }
    }
}

void list(int option, FILE *fp) {
    table_top(headers[option], &headers_count[option], &cell_width[option]);
    if (fp == NULL) {
        printf("No data file found.\n");
        return;
    }
    if (!option) {
        Account account;
        fseek(fp, 0, SEEK_SET);
        while (fread(&account, sizeof(Account), 1, fp) == 1) {
            char balance[50];
            snprintf(balance, 50, "%.2f", account.balance);
            const char *content[] = {account.accId,   account.name,
                                     account.surname, account.address,
                                     account.pesel,   balance};
            content_cell(content, &headers_count[option], &cell_width[option]);
        }
    } else {
        Insurance insurance;
        fseek(fp, 0, SEEK_SET);
        while (fread(&insurance, sizeof(Insurance), 1, fp) == 1) {
            char price[50];
            snprintf(price, 50, "%.2f", insurance.price);
            const char *content[] = {insurance.insuraceId, insurance.accId,
                                     insurance.regNumber, price};
            content_cell(content, &headers_count[option], &cell_width[option]);
        }
    }
    char c;
    while (1) {
        printf("Exit? (y/Y): ");
        scanf("%c", &c);
        while (getchar() != '\n') {
        }
        if (c == 'y' || c == 'Y')
            break;
    }
}
