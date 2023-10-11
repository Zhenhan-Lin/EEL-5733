## EEL5733 Assignment 1

### Compile & Test

+ To build the project, run `make` (alias of `make all`).

+ Use samples in `./sample` to run executable files

```
./email_filter.bin < sample/sample1-1.txt 
./calendar_filter.bin < sample/sample2-1.txt   
./location_updater.bin < sample/sample3-1.txt
```

+ Use `diff` to verify correctness
``` 
./location_updater.bin < sample/sample3-1.txt 1> test_out3-1.txt
diff test_out3-1.txt out3-1.txt
```

### Folder structure

```plaintext
~/assignment 1$ tree
	.
	├── Makefile
	├── README.md
	├── calendar_filter.c
	├── email_filter.c
	├── location_updater.c
	└── sample
		├── out1-1.txt
		├── out2-1.txt
		├── out3-1.txt
		├── output1-1.txt
		├── sample1-1.txt
		├── sample2-1.txt
		├── sample2-2.txt
		├── sample2-3.txt
		├── sample2-4.txt
		├── sample2-5.txt
		├── sample2-6.txt
		├── sample2-7.txt
		├── sample2-8.txt
		└── sample3-1.txt
```
