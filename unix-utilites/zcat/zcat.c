#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
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
