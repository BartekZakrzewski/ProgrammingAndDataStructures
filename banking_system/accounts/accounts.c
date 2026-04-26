#include "accounts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Account read_account(FILE *fp) {
    Account account;

    generateUniqueId(fp, account.accId);
    read_string(create_account[0], account.name, NAME_LEN);
    read_string(create_account[1], account.surname, NAME_LEN);
    read_string(create_account[2], account.address, ADDR_LEN);
    read_string(create_account[3], account.pesel, PESEL_LEN);

    char balance[32];
    read_string(create_account[4], balance, sizeof(balance));
    account.balance = atof(balance);

    return account;
}

void transfer(FILE *fp) {
    char fromId[6], toId[6];
    double amount;

    printf("From account ID: ");
    fgets(fromId, sizeof(fromId), stdin);
    fromId[strcspn(fromId, "\n")] = '\0';

    printf("To account ID: ");
    fgets(toId, sizeof(toId), stdin);
    toId[strcspn(toId, "\n")] = '\0';

    printf("Amount: ");
    scanf("%lf", &amount);
    while (getchar() != '\n') {
    }

    Account fromAccount, toAccount;
    int fromFound = 0, toFound = 0;

    fseek(fp, 0, SEEK_SET);
    while (fread(&fromAccount, sizeof(Account), 1, fp) == 1) {
        if (strcmp(fromAccount.accId, fromId) == 0) {
            fromFound = 1;
            break;
        }
    }

    fseek(fp, 0, SEEK_SET);
    while (fread(&toAccount, sizeof(Account), 1, fp) == 1) {
        if (strcmp(toAccount.accId, toId) == 0) {
            toFound = 1;
            break;
        }
    }

    if (!fromFound || !toFound) {
        printf("One or both account IDs not found.\n");
        return;
    }

    if (fromAccount.balance < amount) {
        printf("Insufficient funds in the source account.\n");
        return;
    }

    char confirm;
    printf("Confirm transfer of %.2f from account %s to account %s? (y/n): ",
           amount, fromAccount.accId, toAccount.accId);
    scanf("%c", &confirm);
    while (getchar() != '\n') {
    }
    if (confirm != 'y' && confirm != 'Y') {
        printf("Transfer cancelled.\n");
        return;
    }

    fromAccount.balance -= amount;
    toAccount.balance += amount;

    fseek(fp, -sizeof(Account), SEEK_CUR);
    fwrite(&toAccount, sizeof(Account), 1, fp);

    fseek(fp, -sizeof(Account) * 2, SEEK_CUR);
    fwrite(&fromAccount, sizeof(Account), 1, fp);

    printf("Transfer successful.\n");
}
