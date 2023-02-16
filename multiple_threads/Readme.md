## EEL5733 Assignment 2

### Compile & Test

+ To build the project, run `make` (alias of `make all`).
+ > Run `make run` or `make run BUFFER=?` in the terminal
  >
  > default buffer size: `10`  
  > default sample input: `input.txt`
  
+ To build the project, run `make clean`

### Folder structure

```plaintext
~/multiple_threads $ tree
    .
    ├── Makefile
    ├── Readme.md
    ├── inc
    │   ├── calendar_filter_t.c
    │   └── email_filter_t.c
    ├── input.txt
    ├── lib
    │   ├── queue.c
    │   ├── queue.h
    │   ├── thread.c
    │   ├── thread.h
    │   └── util.h
    └── location_updater.c