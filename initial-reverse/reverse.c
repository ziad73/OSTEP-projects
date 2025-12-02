#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *in = NULL;
  FILE *out = NULL;

  if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
    printf("error: input and output file must differ\n");
    exit(1);
  }

  if (argc == 1) {
    // ./reverse  --> stdin to stdout
    in = stdin;
    out = stdout;
  } else if (argc == 2) {
    // ./reverse input.txt  --> file to stdout
    in = fopen(argv[1], "r");
    out = stdout;
  } else if (argc == 3) {
    // ./reverse input.txt output.txt  --> file to file
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
  } else {
    printf("usage: reverse [input [output]]\n");
    exit(1);
  }

  if (in == NULL) {
    printf("File not found\n");
    exit(1);
  }

  if (argc == 3 && out == NULL) {
    printf("error: cannot open output file\n");
    fclose(in);
    exit(1);
  }

  // we cannot use fseek with stdin so we have to store all lines then print
  // them reversed
  if (in == stdin) {
    int cnt = 0;
    char buff[1024]; // 1023 chars + null terminator '\0'

    // array of pointers, each pointer points on array of char(buff[1024])
    char *lines[1000]; // 999 line

    while (fgets(buff, sizeof(buff), in)) {
      lines[cnt] = strdup(buff);
      cnt++;
    }
    for (int i = cnt - 1; i >= 0; i--) {
      fputs(lines[i], out);
      free(lines[i]);
    }
    return 0;
  }

  fseek(in, 0, SEEK_END); // EOF
  long pos = ftell(in);
  while (pos > 0) {
    fseek(in, --pos, SEEK_SET); // move back till '\n' then move forward

    // returns next char as int in ASCII stored in [0,255]
    // EOF, '\n', '\0', etc represent as int ASCII values(EOF=-1) so it is okay
    // to compare with them directly
    int c = fgetc(in);

    if (c == '\n') {
      long start = ftell(in); // start pos at last line
      int ch;
      while ((ch = fgetc(in)) != '\n' && ch != EOF) {
        // printf("%c",(char)ch);
        // putchar(ch); // converts int ASCII into char then print it
        fputc(ch, out); // putchar but for file to handle output files
      }
      // putchar('\n');
      fputc('\n', out); // cursor ++
      fseek(in, start, SEEK_SET);
    }
  }

  // when pos=0 (first line)
  fseek(in, 0, SEEK_SET); // start from begin
  int ch;
  while ((ch = fgetc(in)) != '\n' && ch != EOF) {
    // putchar(ch);
    fputc(ch, out);
  }
  // putchar('\n');
  fputc('\n', out);

  if (in != stdin)
    fclose(in);
  if (out != stdout)
    fclose(out);

  return 0;
}
