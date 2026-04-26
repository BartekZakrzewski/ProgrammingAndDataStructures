#include "helpers.h"
#include <stdio.h>

void print_banner(int option) {
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
    if (option == 1)
        printf("Welcome to the banking system!\n");
}

void print_commands(int option) {
    const char **opts = operations[option - 1];
    print_banner(option);
    printf("Available commands: %d\n", option);
    for (size_t i = 0; i < operations_count[option - 1]; i++) {
        printf("%s", operations[option - 1][i]);
    }
}

int refresh = 1;
int read_command(int *option) {
#ifdef _WIN32
    if (refresh)
        system("cls");
#else
    if (refresh)
        system("clear");
#endif
    print_commands(*option);
    printf("Choose option: ");
    char c;
    scanf("%c", &c);
    while (getchar() != '\n')
        ;
    if (c < '1' || c > operations_count[*option - 1] + '0') {
        printf("Invalid command. Please try again.\n");
        return read_command(option);
    }
    return c - '0';
}

void read_string(const char *prompt, char *buffer, int max_len) {
    printf("%s", prompt);
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void generateId(char *id) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 5; i++)
        id[i] = charset[rand() % 36];
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

void generateUniqueId(FILE *fp, char *id) {
    do {
        generateId(id);
    } while (idExists(fp, id));
}
