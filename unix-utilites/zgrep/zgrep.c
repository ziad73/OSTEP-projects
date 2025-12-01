
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void search_and_print(FILE *fp, char *str, int strSize) {
  char *buff = NULL;
  size_t size = 0;

  while (getline(&buff, &size, fp) != -1) {
    int buffSize = strlen(buff);

    for (int i = 0; i <= buffSize - strSize; i++) {
      if (buff[i] == str[0]) {
        bool matched = true;

        for (int j = 1; j < strSize; j++) {
          if (buff[i + j] != str[j]) {
            matched = false;
            break;
          }
        }

        if (matched) {
          printf("%s", buff); // buff already contains newline
          break;
        }
      }
    }
  }

  free(buff);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);
  }

  char str[100];
  strcpy(str, argv[1]);
  int strSize = strlen(str);

  // If search term is empty
  if (strSize == 0) {
    printf("Cannot search without search word\n");
    exit(1);
  }

  // If no files -> read from stdin
  if (argc == 2) {
    search_and_print(stdin, str, strSize);
    return 0;
  }

  // Otherwise loop through all files
  for (int k = 2; k < argc; k++) {
    FILE *fp = fopen(argv[k], "r");

    if (fp == NULL) {
      printf("wgrep: cannot open file\n");
      continue;
    }

    search_and_print(fp, str, strSize);

    fclose(fp);
  }

  return 0;
}
