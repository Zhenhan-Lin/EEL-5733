//
// Created by 林真含 on 2023/2/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/calendar_filter_t.c"
#include "inc/email_filter_t.c"
#include "lib/queue.h"
#include "lib/util.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    size_t MAXSIZE = atoi(argv[1]);
    process_init();
	// Initialize queue
	InitQueue(MAXSIZE);
    pid_t child_p[2];        // PID create for child process

    switch (child_p[0] = fork()) {
        case -1:
            errExit("child 1 create error\n");
        case 0:
            email_filter(NULL);
            exit(0);
        default:
            break;
    }

    switch (child_p[1] = fork()) {
        case -1:
            errExit("child 2 create error\n");
        case 0:
            calendar_filter(NULL);
            exit(0);
        default:
            break;
    }

	// Wait for created thread to exit
    if (waitpid(child_p[0], NULL, 0) == -1)
        errExit("wait child 1\n");
    if (waitpid(child_p[1], NULL, 0) == -1)
        errExit("wait child 2\n");

    return 0;
}