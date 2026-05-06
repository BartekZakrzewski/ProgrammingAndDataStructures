#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int option = 0;

    while (1) {
        scanf("%d", &option);
        if (option < 1 || option > 4) {
            printf("Choose option between 1 and 4\n");
            continue;
        }
    }

    return 0;
}
