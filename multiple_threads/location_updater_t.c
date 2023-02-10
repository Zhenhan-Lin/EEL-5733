//
// Created by 林真含 on 2023/2/9.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define errExit(__VA_ARGS__){ \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);              \
}

int main(int argc, char *argv[]){
    size_t MAXSIZE = atoi(argv[1]);

    pthread_t threads[2];
}