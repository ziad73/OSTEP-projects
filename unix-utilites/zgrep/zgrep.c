
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

  int out = -1;
  bool append = 0;
  // detect > or >>
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0) {
      // no assign file
      if (i + 1 >= argc) {
        printf("wzip: missing output file\n");
        exit(1);
      }
      append = strcmp(argv[i], ">>") == 0;

      if (append) {
        out = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND,
                   0644); // 0644 -rw-r--r--
      } else
        out = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
                   0644); // 0644 -rw-r--r--

      if (out < 0) {
        printf("error at openning output file\n");
        exit(1);
      }

      // replace stdout file descriptor "1" with out value
      dup2(out, STDOUT_FILENO); // sdtout file # is 1
      close(out);

      argc = i; // only files before > or >> are input files
      break;
    }
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
