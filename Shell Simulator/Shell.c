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
#define errExit(__VA_ARGS__){ \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);              \
        }
//#define STD_OUT    ">"   // redirect std input to the file
//#define STD_IN     "<"   // redirect std input to the prog
//#define PIPE       "|"   // execute prog1 and prog2 in parallel and redirect std output of prog1 to prog2
//#define SEQUENTIAL ";"   // first execute prog1 in a process and once it terminates execute prog2 in a process
//#define CD_AND     "&&"  // first execute prog1 in a process and if the return status is 0 then execute prog2 in a process
//#define CD_OR      "||"  // first execute prog1 in a process and if the return status is not zero then execute prog2

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
    char *args1[ARG_MAX] = {NULL};  //有限长度的指针
    char *args2[ARG_MAX] = {NULL};
    str = (char *)malloc(BUFFER_SIZE * sizeof(char));


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

        pid_t child_p;        // PID create for child process
        switch (child_p = fork()) {
            case -1:
                errExit("child 1 create error\n");
            case 0:
                execvp(args1[0], args1);

                errExit("execvp email_filter\n");
            default:
                break;
        }

//        int status = 0;
//        waitpid(child_p, &status, 0);
//        errExit("wait child 1\n");
    }
}
