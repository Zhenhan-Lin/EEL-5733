//
// Created by 林真含 on 2023/1/28.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ  0
#define WRITE 1
#define PROCESS_1 "./email_filter.bin"
#define PROCESS_2 "./calendar_filter.bin"
#define errExit(__VA_ARGS__){ \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);              \
        }


int main(int argc, char *argv[])
{
    int pfd[2];              // Pipe file descriptors
    pipe(pfd);               // email_filter - fifo - calendar_filter
    pid_t child_p[2];        // PID create for child process
    if (pipe(pfd) == -1)
        errExit("pipe create error\n");

    switch (child_p[0] = fork()) {
        case -1:
            errExit("child 1 create error\n");

        case 0:             /* First child: exec 'ls' to write to pipe */
            if (close(pfd[READ]) == -1)                    /* READ end is unused */
                errExit("child 1: close read end error\n");

            /* Duplicate stdout on write end of pipe; close duplicated descriptor */

            if (pfd[WRITE] != STDOUT_FILENO) {              /* Defensive check */
                dup2(pfd[WRITE], STDOUT_FILENO);
                if (dup2(pfd[WRITE], STDOUT_FILENO) == -1)
                    errExit("child 1: dup2 write error\n");
                if (close(pfd[WRITE]) == -1)
                    errExit("child 1: close write end error\n");
            }

            execl(PROCESS_1, (char *) NULL);          /* Writes to pipe */
            errExit("execl email_filter\n");

        default:            /* Parent falls through to create next child */
            break;
    }

    switch (child_p[1] = fork()) {
        case -1:
            errExit("child 2 create error\n");

        case 0:             /* Second child: exec 'wc' to read from pipe */
            if (close(pfd[WRITE]) == -1)                    /* WRITE end is unused */
                errExit("child 2: close write end error\n");

            /* Duplicate stdin on read end of pipe; close duplicated descriptor */

            if (pfd[READ] != STDIN_FILENO) {               /* Defensive check */
                dup2(pfd[READ], STDIN_FILENO);
                if (dup2(pfd[READ], STDIN_FILENO) == -1)
                    errExit("child 2: dup2 read error\n");
                if (close(pfd[READ]) == -1)
                    errExit("child 2: close read end error\n");
            }

            execl(PROCESS_2, (char *) NULL);
            errExit("execl calendar_filter\n");

        default:
            break;
    }

    if (close(pfd[READ]) == -1)
        errExit("close read end error\n");
    if (close(pfd[WRITE]) == -1)
        errExit("close write end error\n");
    if (wait(NULL) == -1)
        errExit("wait child 1\n");
    if (wait(NULL) == -1)
        errExit("wait child 2\n");

    exit(EXIT_SUCCESS);
}