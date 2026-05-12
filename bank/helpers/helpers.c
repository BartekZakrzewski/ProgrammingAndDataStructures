#include "helpers.h"

/** Local functions **/
static int validate_letters(char *str) {
    for (int c = 0; c < (int)strlen(str); c++) {
        if (!(str[c] >= 'a' && str[c] <= 'z') && !(str[c] >= 'A' && str[c] <= 'Z'))
            return 1;
    }
    return 0;
}

void generateId(char *id) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 5; i++) {
        id[i] = charset[rand() % 36];
    }
    id[5] = '\0';
}

int idExists(FILE *fp, char *id) {
    Account acc;
    fseek(fp, 0, SEEK_SET);
    while (fread(&acc, sizeof(Account), 1, fp) == 1)
        if (strcmp(acc.accId, id) == 0)
            return 1;
    return 0;
}

/** Public functions **/

/* Helpers */
void read_string(const char *prompt, char *buffer, int max_len) {
    printf("%s", prompt);
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void generateUniqueId(FILE *fp, char *id) {
    do {
        generateId(id);
    } while (idExists(fp, id));
}

int refresh = 1;
void __refresh() {
    // clang-format off
    #ifdef _WIN32
        if (refresh)
            system("cls");
    #else
        if (refresh)
            system("clear");
    #endif
    // clang-format on
}

void print_banner(int show_welcome) {
    const char *banner =
        " ________  ________  ________   ___  __        ________     \n"
        "|\\   __  \\|\\   __  \\|\\   ___  \\|\\  \\|\\  \\     |\\   ____\\  "
        "  "
        "\n"
        "\\ \\  \\|\\ /\\ \\  \\|\\  \\ \\  \\\\ \\  \\ \\  \\/  /|_   \\ \\  "
        "\\___|    \n"
        " \\ \\   __  \\ \\   __  \\ \\  \\\\ \\  \\ \\   ___  \\   \\ \\  \\  "
        "  "
        "   \n"
        "  \\ \\  \\|\\  \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\ \\  \\ __\\ "
        "\\  \\____  \n"
        "   \\ \\_______\\ \\__\\ \\__\\ \\__\\\\ \\__\\ \\__\\\\ \\__\\\\__\\ "
        "\\_______\\\n"
        "    \\|_______|\\|__|\\|__|\\|__| \\|__|\\|__| "
        "\\|__\\|__|\\|_______|\n";

    printf("%s\n", banner);
    if (show_welcome == 1)
        printf("Welcome to the banking system!\n");
}

int action(int option, int options_len) {
    int _opt;
    printf("Avaliable actions (1 - %d)\n", options_len);
    for (int opt = 0; opt < options_len; opt++) {
        printf("%s\n", options[option - (option / 2)][opt]);
    }
    printf("\nOption: ");
    scanf("%d", &_opt);
    char c;
    while ((c = getchar()) != '\n') {
    }
    while (_opt < 1 || _opt > options_len) {
        printf("Choose option between 1 and %d\n", options_len);
        printf("\nOption: ");
        scanf("%d", &_opt);
    }
    return _opt;
}

/* Validators */
int validate_account(Account account) {
    if (validate_letters(account.name) || validate_letters(account.surname))
        return -1;

    if (strlen(account.pesel) != 11)
        return -2;

    for (int c = 0; c < (int)strlen(account.pesel); c++) {
        if (!(account.pesel[c] >= '0' && account.pesel[c] <= '9'))
            return -2;
    }

    if (account.balance < 0)
        return -3;

    return 0;
}

int validate_insurance(Insurance insurance, FILE *accounts) {
    int is_valid_acc = 0;
    Account acc;
    fseek(accounts, 0, SEEK_SET);
    while (fread(&acc, sizeof(Account), 1, accounts) == 1) {
        if (strcmp(acc.accId, insurance.accId) == 0) {
            is_valid_acc = 1;
            break;
        }
    }
    if (!is_valid_acc)
        return -1;

    for (int c = 0; c < (int)strlen(insurance.regNumber); c++) {
        if (!(insurance.regNumber[c] >= 'A' && insurance.regNumber[c] <= 'Z') && !(insurance.regNumber[c] >= '0' && insurance.regNumber[c] <= '9'))
            return -2;
    }

    if (insurance.price < 0)
        return -3;
    return 0;
}

/* Misc */
Insurance _create_insurance(FILE *insurances) {
    Insurance insurance = {0};

    generateUniqueId(insurances, insurance.insuraceId);
    read_string(create_insurance[0], insurance.accId, 20);
    read_string(create_insurance[1], insurance.regNumber, REG_LEN);

    char price[32];
    read_string(create_insurance[2], price, sizeof(price));
    insurance.price = atof(price);

    return insurance;
}

Account _create_account(FILE *fp) {
    Account account = {0};

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

void __exit() {
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
