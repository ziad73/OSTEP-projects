#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("No files to compress\n");
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

  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Cannot open file\n");
    exit(1);
  }

  char buff[100];
  while (fgets(buff, 100, fp)) {
    printf("%s", buff);
  }
  fclose(fp);
  printf("\n");
}
