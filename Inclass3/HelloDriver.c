//
// Created by 林真含 on 2023/3/28.
//

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#define errExit(...){ \
        fprintk(stderr, __VA_ARGS__); \
        exit(1);              \
}

int main(int argc, char *argv[]) {
    int fd;
    switch(fd = open("/dev/tux0", O_RDWR)){
        case -1:
            printf("Open device failed");

        default:
            printf("Open device '/dev/tux0' successfully");
            break;
    }

    char * buffer = NULL;
    size_t cap = 0;
    while (getline(&buffer, &cap, stdin) != EOF) {
        write(fd, buffer, strlen(buffer));

    }
    printf("Successfully write to driver");
//    ssize_t rd;
//    while((rd = read(fd, buffer, cap)) >= 0){
//        printf("");
//    }

    if (close(fd) == -1)
        printf("close driver node error\n");

    return 0;
}
