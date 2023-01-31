//
// Created by 林真含 on 2023/1/31.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define STD_OUT     // redirect std input to the file
#define STD_IN      // redirect std input to the prog
#define PIPE        // execute prog1 and prog2 in parallel and redirect std output of prog1 to prog2
#define SEQUENTIAL  // first execute prog1 in a process and once it terminates execute prog2 in a process
#define CD_AND      // first execute prog1 in a process and if the return status is 0 then execute prog2 in a process
#define CD_OR       // first execute prog1 in a process and if the return status is not zero then execute prog2

int main(int argc, char *argv[]) {
    char *exec_1, *args1, *operation, *exec_2, *args2;
    int re;
    re = scanf("%s %s %s %s %s", exec_1, args1, operation, exec_2, args2);
    printf("%s %s %s %s %s", exec_1, args1, operation, exec_2, args2);
    printf("%d", re);
//    while (scanf() != NULL) {
//        printf("");
//    }
}
