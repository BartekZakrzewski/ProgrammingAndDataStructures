#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>

void deleteWhiteSpaces(char *line, int *line_size);
int sanitize(char *line, int line_size);
void freeNumbers(char **numbers, int n_numbers);

int saveLine(char ***numbers, int *numbers_size, int *n_numbers, char *line,
             int *line_size);
char *getLine(int *line_size);

char *sum(char **numbers, int n_numbers);

int main(int argc, char *argv[]) {
  int n_numbers = 1;
  int numbers_size = 1;
  char **numbers = (char **)malloc(sizeof(char *) * n_numbers);
  if (numbers == NULL) {
    printf("numbers init error\n");
    return 1;
  }
  numbers[0] = NULL;

  int line_size;
  char *line;
  while ((line = getLine(&line_size)) != NULL) {
    int error_code =
        saveLine(&numbers, &numbers_size, &n_numbers, line, &line_size);
    if (error_code == -1) {
      free(line);
      printf("Incorrect input\n");
      return 1;
    }
    if (error_code == -2) {
      free(line);
      freeNumbers(numbers, n_numbers - 1);
      printf("Error while saving line\n");
      return 1;
    }
    free(line);
  }
  char *result = sum(numbers, n_numbers - 1);
  if (result == NULL) {
    freeNumbers(numbers, n_numbers - 1);
    printf("Error while summing numbers\n");
    return 1;
  }
  printf("Result:\n%s\n", result);

  printf("\nInput numbers:\n");
  for (int i = 0; i < n_numbers - 1; i++) {
    printf("%s\n", numbers[i]);
  }
  free(numbers);

  return 0;
}

void deleteWhiteSpaces(char *line, int *line_size) {
  int is_number = 0;
  for (int i = 0; i < *line_size; i++) {
    if (line[i] == ' ' && is_number) {
      line[i] = '\0';
      *line_size = i;
      return;
    }

    if (line[i] == ' ') {
      for (int j = i; j < *line_size - 1; j++) {
        line[j] = line[j + 1];
      }
      line[*line_size - 1] = '\0';
      *line_size -= 1;
      i--;
    } else {
      is_number = 1;
    }
  }
}

int sanitize(char *line, int line_size) {
  for (int i = 0; i < line_size; i++) {
    if (line[i] == ' ' || line[i] == '\n') {
      continue;
    }
    if (line[i] < '0' || line[i] > '7') {
      return 0;
    }
  }
  return 1;
}

void freeNumbers(char **numbers, int n_numbers) {
  for (int i = 0; i < n_numbers; i++) {
    free(numbers[i]);
  }
  free(numbers);
}

int saveLine(char ***numbers, int *numbers_size, int *n_numbers, char *line,
             int *line_size) {
  if (!sanitize(line, *line_size)) {
    return -1;
  }
  deleteWhiteSpaces(line, line_size);
  if (*line_size <= 0) {
    return 0;
  }
  // char *new_line = (char *)realloc(line, sizeof(char *) * (*line_size));
  // if (new_line == NULL) {
  //   return -2;
  // }
  // printf("Debug %s | %d\n", line, *line_size);
  // line = new_line;
  // printf("Debug %s\n", line);
  // printf("\nDEBUG: %s | %lu\n", line, sizeof(char *) * sizeof(line));
  char *new_numbers = (char *)realloc((*numbers)[*n_numbers - 1],
                                      sizeof(char *) * (*line_size + 1));
  if (new_numbers == NULL) {
    return -2;
  }
  (*numbers)[*n_numbers - 1] = new_numbers;
  for (int i = 0; i < *line_size; i++) {
    (*numbers)[*n_numbers - 1][(*numbers_size) - 1] = line[i];
    (*numbers_size)++;
  }
  (*numbers)[*n_numbers - 1][(*numbers_size) - 1] = '\0';
  *(n_numbers) += 1;
  *(numbers_size) = 1;
  char **new_numbers_array =
      (char **)realloc(*numbers, sizeof(char *) * (*n_numbers));
  if (new_numbers_array == NULL) {
    return -2;
  }
  (*numbers) = new_numbers_array;
  (*numbers)[*n_numbers - 1] = NULL;
  return 1;
}

char *getLine(int *len) {
  char character;
  int character_index = 0;
  int line_size = 1;
  char *line = (char *)malloc(line_size * sizeof(char));
  if (line == NULL) {
    return NULL;
  }
  while ((character = getchar()) != '\n' && character != EOF) {
    line[character_index++] = (char)character;
    if (character_index >= line_size) {
      char *new_line = (char *)realloc(line, sizeof(char) * (line_size * 2));
      if (new_line == NULL) {
        free(line);
        return NULL;
      }
      line = new_line;
      line_size *= 2;
    }
  }

  if (character_index == 0 && character == EOF) {
    free(line);
    return NULL;
  }

  char *new_line = (char *)realloc(line, sizeof(char) * (character_index + 1));
  if (new_line == NULL) {
    free(line);
    return NULL;
  }
  line = new_line;

  line[character_index] = '\0';

  *len = character_index;
  return line;
}

char *sum(char **numbers, int n_numbers) {
  int *lengths = (int *)malloc(sizeof(int) * n_numbers);
  if (lengths == NULL) {

    return NULL;
  }
  int max_length = 0;
  for (int i = 0; i < n_numbers; i++) {
    lengths[i] = 0;
    while (numbers[i][lengths[i]] != '\0') {
      lengths[i]++;
    }
    if (lengths[i] > max_length) {
      max_length = lengths[i];
    }
  }
  char *result = (char *)malloc(sizeof(char) * (max_length + 2));
  if (result == NULL) {
    free(lengths);
    return NULL;
  }
  int carry = 0;
  for (int i = 0; i < max_length; i++) {
    int column_sum = carry;
    for (int j = 0; j < n_numbers; j++) {
      if (lengths[j] - 1 - i >= 0) {
        column_sum += numbers[j][lengths[j] - 1 - i] - '0';
      }
    }
    result[max_length - i] = (char)(column_sum % 8 + '0');
    carry = column_sum / 8;
  }
  if (carry > 0) {
    result[0] = (char)(carry + '0');
    result[max_length + 1] = '\0';
  } else {
    for (int i = 0; i < max_length; i++) {
      result[i] = result[i + 1];
    }
    result[max_length] = '\0';
  }
  while (*result == '0' && *(result + 1) != '\0') {
    result++;
  }
  return result;
}
