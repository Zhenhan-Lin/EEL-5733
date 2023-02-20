//
// Created by 林真含 on 2023/2/19.
//

#include "lib/account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lib/util.h"
#include "lib/transfer.c"

size_t acc_len = 0;
Account acc_list = (Account)NULL;

int main(int argc, char *argv[]){
    size_t len = 0;
    int worker;
    char *input = NULL;

    char *pattern_1 = "^([0-9]+) ([0-9]+)";
    char *pattern_2 = "^Transfer ([0-9]+) ([0-9]+) ([0-9]+)";

    if (argc >= 2){
        freopen(argv[1], "r", stdin);
        worker = atoi(argv[2]);
    }
    while (getline(&input, &len, stdin) >= 0) {
        for (char *c = input; *c; c++) if (*c=='\n') *c = ' ';
        if(match_regex(pattern_1, input) == true){
            // Expand list space
            acc_list = (Account)realloc((void *)acc_list, (++acc_len) * sizeof(*acc_list));
            Account a = acc_list + acc_len - 1;
            a->number = atol(strtok(input, " "));
            a->balance = atol(strtok(NULL, " "));
            a->lock = malloc(sizeof(*(a->lock)));
            pthread_mutex_init(a->lock, NULL);
        }else if(match_regex(pattern_2, input) == true){
            // Initialize all threads
            pthread_t tid[worker];
            for(int i=0;i<worker;i++)
                pthread_create(&tid[i], NULL, thread_main, NULL);
        }else{
            errExit("Wrong input format\n");
        }
    }
//    char *ptr = strtok(input, " ");
//    size_t draw, withdraw, amount;
//    draw = atol(strtok(NULL, " "));
//    withdraw = atol(strtok(NULL, " "));
//    amount = atol(strtok(NULL, " "));
}
