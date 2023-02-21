## EEL5733 Assignment 3

### Compile & Test

+ To build the project, run `make` (alias of `make all`).
+ Run `make run` or `make run BUFFER=?` in the terminal
  >
  > default buffer size: `10`  
  > default sample input: `sample/input.txt`
+ To run other input files, please use format like: 
  > basic command: `/tranfProg [input_file_name] [buffer_size]`
  > compare with result:  add `| diff -u [result_file_name] - `
  > reminder： add `&& echo "[PASS]" || echo "[FAIL]"`
+ To build the project, run `make clean`

### Folder structure

```plaintext
~/EFT program $ tree
  .
  ├── Makefile
  ├── Readme.md
  ├── inc
  │   └── transfer.c
  ├── lib
  │   ├── account.c
  │   ├── account.h
  │   └── util.h
  ├── sample
  │   ├── EFT.1.in
  │   ├── EFT.1.out
  │   ├── input.txt
  │   └── output.txt
  └── tranfProg.c
