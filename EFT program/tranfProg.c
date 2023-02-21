//
// Created by 林真含 on 2023/2/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lib/util.h"
#include "lib/account.h"
#include "inc/transfer.c"

size_t acc_len = 0;
Account acc_list = (Account)NULL;

int main(int argc, char *argv[]){
    size_t len = 0;
    int num_thread = 10;
    char *input = NULL;

    char *pattern_1 = "^([0-9]+) ([0-9]+)";

    if(argc >= 2){
        freopen(argv[1], "r", stdin);
        num_thread = atoi(argv[2]);
    }else{
        freopen("/Users/linzhenhan/Documents/GitHub/EEL5733/EFT program/sample/input.txt", "r", stdin);
    }

    pthread_t *tid = malloc(num_thread * sizeof(*tid));
    while(getline(&input, &len, stdin) >= 0){
        if(match_regex(pattern_1, input) == true){
            // Expand list space
            for(char *c = input; *c; c++) if (*c=='\n') *c = ' ';
            acc_list = (Account)realloc((void *)acc_list, (++acc_len) * sizeof(*acc_list));
            Account a = acc_list + acc_len - 1;
            a->number = atol(strtok(input, " "));
            a->balance = atol(strtok(NULL, " "));
            a->lock = malloc(sizeof(*(a->lock)));
            pthread_mutex_init(a->lock, NULL);
        }else if(match_regex(pattern_2, input) == true){
//            char *ptr = strtok(input, " ");
//            size_t sender, reciever;
//            long amount;
//            sender = atol(strtok(NULL, " "));
//            reciever = atol(strtok(NULL, " "));
//            amount = atol(strtok(NULL, " "));
            for(size_t i = 0; i < num_thread; i++)
                pthread_create(&tid[i], NULL, thread_main, (i == 0) ? input : NULL);
            break;
        }else{
            continue;
//            errExit("Wrong input format\n");
        }
    }

    for(size_t i = 0; i < num_thread; i++)
        pthread_join(tid[i], NULL);
//     Print updated account balances
    for(size_t i = 0; i < acc_len; i++)
        printf("%zu %ld\n", acc_list[i].number, acc_list[i].balance);
    return 0;
}
