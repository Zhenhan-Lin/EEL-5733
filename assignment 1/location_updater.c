//
// Created by 林真含 on 2023/1/28.
//


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define Read  0
#define Write 1

int main(int argc, char *argv[])
{
    int pfd[2];           /* Pipe file descriptors */

    /* Create pipe */
    pipe(pfd);    // email_filter - fifo - calendar_filter

    pid_t child_p[2];
//    if (pipe(pfd) == -1)                            /* Create pipe */
//        errExit("pipe");

    switch (child_p[0] = fork()) {
//        case -1:
//            errExit("fork");

        case 0:             /* First child: exec 'ls' to write to pipe */
            close(pfd[Read]);

//            if (close(pfd[0]) == -1)                    /* Read end is unused */
//                errExit("close 1");

            /* Duplicate stdout on write end of pipe; close duplicated descriptor */

            if (pfd[Write] != STDOUT_FILENO) {              /* Defensive check */
                dup2(pfd[Write], STDOUT_FILENO);
//                if (dup2(pfd1[1], STDOUT_FILENO) == -1)
//                    errExit("dup2 1");
                close(pfd[Write]);
//                if (close(pfd1[1]) == -1)
//                    errExit("close 2");
            }

            execlp("EmailFilter", "EmailFilter", (char *) NULL);          /* Writes to pipe */
//            errExit("execlp ls");

        default:            /* Parent falls through to create next child */
            break;
    }

    switch (child_p[1] = fork()) {
//        case -1:
//            errExit("fork");

        case 0:             /* Second child: exec 'wc' to read from pipe */
            close(pfd[Write]);
//            if (close(pfd2[1]) == -1)                    /* Write end is unused */
//                errExit("close 3");

            /* Duplicate stdin on read end of pipe; close duplicated descriptor */

            if (pfd[Read] != STDIN_FILENO) {               /* Defensive check */
                dup2(pfd[Read], STDIN_FILENO);
//                if (dup2(pfd2[0], STDIN_FILENO) == -1)
//                    errExit("dup2 2");
                close(pfd[Read]);
//                if (close(pfd2[0]) == -1)
//                    errExit("close 4");
            }

            execlp("CalendarFilter", "CalendarFilter", (char *) NULL);
//            errExit("execlp wc");

        default: /* Parent falls through */
            break;
    }

    /* Parent closes unused file descriptors for pipe, and waits for children */

    close(pfd[0]);
    close(pfd[1]);
    wait(NULL);
    wait(NULL);
//    if (close(pfd1[0]) == -1)
//        errExit("close 5");
//    if (close(pfd2[1]) == -1)
//        errExit("close 6");
//    if (wait(NULL) == -1)
//        errExit("wait 1");
//    if (wait(NULL) == -1)
//        errExit("wait 2");

//    exit(EXIT_SUCCESS);
}