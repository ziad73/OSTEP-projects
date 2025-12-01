#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wunzip: file1 [file2 ...]\n");
    exit(1);
  }

  // file descriptor that represents an open file in the operating system(0
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

  // Now decompress files

  // multiple files
  for (int i = 1; i < argc; i++) {
    FILE *ptr = fopen(argv[i], "r");

    if (ptr == NULL) {
      printf("Cannot open file\n");
      continue;
    }
    char c;
    int cnt = 0;

    while (fread(&cnt, sizeof(int), 1, ptr) == 1 &&
           fread(&c, sizeof(char), 1, ptr) == 1) {
      for (int j = 0; j < cnt; j++) {
        putchar(c);
      }
    }
    fclose(ptr);
  }

  return 0;
}
