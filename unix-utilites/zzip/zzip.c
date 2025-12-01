#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("No files to compress\n");
    exit(1);
  }

  // file descriptor that represents an open file in the operating system(0
  // stdin, 1 stdout, 2 stderr)
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
        // creates argv[i+1] file with permissions -rw-r--r--
        // out stores file descriptor of open
        // 3 or greater on success
        // -1 on failure
        // dup2() uses this to tell the OS: Replace the standard output file
        // descriptor (1) with out after dup2 line any kind of writting/printing
        // will write on opened file
        out = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND,
                   0644); // 0644 -rw-r--r--
      } else
        out = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
                   0644); // 0644 -rw-r--r--

      if (out < 0) {
        printf("error at openning output file\n");
        exit(1);
      }

      // redirect stdout
      // replace stdout file descriptor "1" with out value
      dup2(out, STDOUT_FILENO); // sdtout file # is 1
      close(out);

      argc = i; // only files before > or >> are input files
      break;
    }
  }

  // Now compress files
  int cnt = 0;
  char prev;
  bool frst = 1;

  // multiple files
  for (int i = 1; i < argc; i++) {
    FILE *ptr = fopen(argv[i], "r");

    if (ptr == NULL) {
      printf("Cannot open file\n");
      continue;
    }
    char c;
    while (fread(&c, sizeof(char), 1, ptr) == 1) {
      if (frst) {
        cnt = 1;
        frst = 0;
        prev = c;
      } else if (c == prev)
        cnt++;
      else {
        fwrite(&cnt, sizeof(int), 1, stdout);
        fwrite(&prev, sizeof(char), 1, stdout);
        cnt = 1;
        prev = c;
      }
    }
    fclose(ptr);
  }
  if (!frst) {
    fwrite(&cnt, sizeof(int), 1, stdout);
    fwrite(&prev, sizeof(char), 1, stdout);
  }
  return 0;
}

// int out_fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// printf("Before redirect\n");  // prints to screen
// dup2(out_fd, 1);   // redirect stdout -> out_fd
// printf("After redirect\n");   // goes into result.txt
