#include <stdio.h>

char *getLine();

int main(int argc, char *argv[]) { return 0; }

char *getLine() {
  char *line = (char *)malloc(sizeof(char) * 100);
  char c;
  while ((c = getchar()) != '\n' && c != EOF) {
    line[] = c;
  }
}
