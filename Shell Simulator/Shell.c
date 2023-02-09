//
// Created by 林真含 on 2023/1/31.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1000
#define ARG_MAX 128
#define READ 0
#define WRITE 1
#define errExit(__VA_ARGS__){ \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);              \
        }

enum operation{
    STD_OUT, STD_IN, PIPE, SEQUENTIAL, CD_AND, CD_OR, NONE
}op;

void free_pointer(char **addr, size_t len){
    for(int i = 0;i<len;i++)
        if(addr[i]!=NULL) {
            free(addr[i]);
            addr[i] = NULL;
        }
}

void operation(char *ptr){
    if(strcmp(ptr, ">") == 0)        op = STD_OUT;
    else if(strcmp(ptr, "<") == 0)   op = STD_IN;
    else if(strcmp(ptr, "|") == 0)   op = PIPE;
    else if(strcmp(ptr, ";") == 0)   op = SEQUENTIAL;
    else if(strcmp(ptr, "&&") == 0)  op = CD_AND;
    else if(strcmp(ptr, "||") == 0)  op = CD_OR;
    else                             op = NONE;
}

int main(int argc, char *argv[]) {
    char *str;
    char *args1[ARG_MAX] = {NULL};
    char *args2[ARG_MAX] = {NULL};
    str = (char *)malloc(BUFFER_SIZE * sizeof(char));

    printf("USR $ ");
    while (fgets(str, BUFFER_SIZE, stdin) != NULL) {
        for (char *c = str; *c; c++) if (*c=='\n') *c = ' ';
        bool flag = false;
        free_pointer(args1, ARG_MAX);
        free_pointer(args2, ARG_MAX);
        char *ptr = strtok(str, " ");

        for(int i = 0; ptr; ptr=strtok(NULL, " "), i++){
            if(flag == false){
                operation(ptr);
                if(op != NONE){
                    flag = true;
                    i = -1;
                    continue;
                }else{
                    args1[i] = (char *) malloc((strlen(ptr) + 1) * sizeof(char));
                    strcpy(args1[i], ptr);
                }
            }else{
                args2[i] = (char *) malloc((strlen(ptr) + 1) * sizeof(char));
                strcpy(args2[i], ptr);
            }
        }

        if(op == STD_IN || op == STD_OUT){
            pid_t child_p;        // PID create for child process
            switch (child_p = fork()) {
                case -1: errExit("child 1 create error\n");
                case 0:
                    if (op == STD_IN) {
                        dup2(open(args2[0], O_RDONLY), STDIN_FILENO);
                    } else if (op == STD_OUT) {
                        dup2(open(args2[0], O_CREAT|O_RDWR), STDOUT_FILENO);
                    }
                    execvp(args1[0], args1);
                    errExit("execvp child\n");
                default:
                    break;
            }
            int status;
            waitpid(child_p, &status, 0);
        }else if(op == PIPE){
//            for(int i=0;args1[i]!=NULL;i++)  puts(args1[i]);
//            for(int i=0;args2[i]!=NULL;i++)  puts(args2[i]);
            int pfd[2];              // Pipe file descriptors
            pipe(pfd);               // email_filter - fifo - calendar_filter
            pid_t child_p[2];        // PID create for child process
            if (pipe(pfd) == -1)
            errExit("pipe create error\n");

            switch (child_p[0] = fork()) {
                case -1:
                errExit("child 1 create error\n");

                case 0:
                    if (close(pfd[READ]) == -1)
                    errExit("child 1: close read end error\n");
                    if (pfd[WRITE] != STDOUT_FILENO) {
                        dup2(pfd[WRITE], STDOUT_FILENO);
                        if (dup2(pfd[WRITE], STDOUT_FILENO) == -1)
                        errExit("child 1: dup2 write error\n");
                        if (close(pfd[WRITE]) == -1)
                        errExit("child 1: close write end error\n");
                    }

                    execvp(args1[0], args1);
                    errExit("execl email_filter\n");

                default:
                    break;
            }

            switch (child_p[1] = fork()) {
                case -1:
                errExit("child 2 create error\n");

                case 0:
                    if (close(pfd[WRITE]) == -1)
                    errExit("child 2: close write end error\n");

                    if (pfd[READ] != STDIN_FILENO) {
                        dup2(pfd[READ], STDIN_FILENO);
                        if (dup2(pfd[READ], STDIN_FILENO) == -1)
                        errExit("child 2: dup2 read error\n");
                        if (close(pfd[READ]) == -1)
                        errExit("child 2: close read end error\n");
                    }

                    execvp(args2[0], args2);
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
        }
        else if(op == SEQUENTIAL || op == CD_AND || op == CD_OR){
//            for(int i=0;args1[i]!=NULL;i++)  puts(args1[i]);
//            for(int i=0;args2[i]!=NULL;i++)  puts(args2[i]);
            pid_t child_p[2];
            int status;

            switch (child_p[0] = fork()) {
                case -1: errExit("child 1 create error\n");
                case 0:
                    execvp(args1[0], args1);
                    errExit("execl calendar_filter\n");
                default:
                    break;
            }

            waitpid(child_p[0], &status, 0);
            switch (child_p[1] = fork()) {
                case -1: errExit("child 2 create error\n");
                case 0:
                    if (op == CD_AND){
//                        printf("%d\n", status);
                        if(status != 0) break;
                    } else if (op == CD_OR){
//                        printf("%d\n", status);
                        if(status == 0) break;
                    }
                    execvp(args2[0], args2);
                    errExit("execvp child\n");
                default:
                    break;
            }
            waitpid(child_p[1], &status, 0);
        }else{
            pid_t child_p;        // PID create for child process
            switch (child_p = fork()) {
                case -1: errExit("child 1 create error\n");
                case 0:
                    execvp(args1[0], args1);
                default:
                    break;
            }
            int status;
            waitpid(child_p, &status, 0);
        }
        printf("USR $ ");
    }
}
