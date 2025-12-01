# Unix Utilities: wcat, wgrep, wzip, wunzip

This project contains implementations of basic UNIX-style utilities written in C. The purpose is to gain experience with system-level programming, file handling, and command-line tool design.

## Tools Included

### wcat

Prints the contents of one or more files to standard output.

Usage:

```
./wcat file1 [file2 ...]
```

### wgrep

Searches for a given term in one or more files and prints each matching line.

Usage:

```
./wgrep search-term [file ...]
```

If no file is provided, wgrep reads from standard input.

### wzip

Compresses files using run-length encoding (RLE). Output format consists of a 4-byte integer followed by a single character for each run. Output is written to standard output.

Usage:

```
./wzip file1 [file2 ...] > output.wz
```

### wunzip

Decompresses files created by wzip and prints the result to standard output.

Usage:

```
./wunzip compressed.wz
```

## Compression Format

wzip writes output in binary form as repeated sequences of:

* A 4-byte integer representing the count
* A 1-byte ASCII character

Example input:

```
aaaabbc
```

Represents:

```
4 a
2 b
1 c
```

## Build Instructions

To compile each tool:

```
gcc -o wcat wcat.c
gcc -o wgrep wgrep.c
gcc -o wzip wzip.c
gcc -o wunzip wunzip.c
```

## Example

```
echo "aaabbc" > sample.txt
./wzip sample.txt > sample.wz
./wunzip sample.wz
```

Expected output:

```
aaabbc
```

## Notes

* wzip and wunzip must handle multiple input files.
* If required arguments are missing, each utility prints a usage message and exits with return code 1.
* When compressing multiple files, information about file boundaries is not preserved.

---


