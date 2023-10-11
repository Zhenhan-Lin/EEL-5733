## EEL5733 Assignment 7b
+ Author: Zhenhan Lin
+ UFID: 78947699

### Compile & Test
+ To build the project, run `make` (alias of `make all`).
+ Use `make config` to test if keyboard is ready
+ If keyboard is ready, there should be no output
+ Use `sudo dmesg -C > /dev/null` to clear dmesg
+ Open another terminal and type `sudo dmesg -wH` to view real time message

### Folder Structure

```plaintext
~/assignmet 7$ tree
    .
    ├── binary file
    │   ├── modules.order
    │   ├── Module.symvers
    │   ├── usbkbd.ko
    │   ├── usbkbd.mod
    │   ├── usbkbd.mod.c
    │   ├── usbkbd.mod.o
    │   └── usbkbd.o
    ├── config.sh
    ├── Makefile
    ├── README.md
    └── usbkbd.c
