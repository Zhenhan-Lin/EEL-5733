//
// Created by 林真含 on 2023/2/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "../lib/account.h"
// 0, 1, 2, 3...

char *pattern_2 = "^Transfer ([0-9]+) ([0-9]+) ([0-9]+)";

Account get_account(size_t account_number) {
    // Find account
    size_t i = 0;
    while (acc_list[i].number != account_number) i++;
    return &(acc_list[i]);
}

void parse_and_execute(char * s) {
//    if(match_regex(pattern_2, s) == false)
//        return;
    for(char *c = s; *c; c++) if (*c=='\n') *c = ' ';

    size_t draw, receive;
    long amount;
    strsep(&s, " ");
    draw = atol(strsep(&s, " "));
    receive = atol(strsep(&s, " "));
    amount = atol(strsep(&s, " "));

    if (draw == receive) return;

    Account
        a = get_account(draw),
        b = get_account(receive);
    pthread_mutex_lock(a->lock);
    while (pthread_mutex_trylock(b->lock) != 0)
        pthread_cond_wait(b->cond, a->lock);
    a->balance -= amount;
    b->balance += amount;
    pthread_mutex_unlock(b->lock);
    pthread_cond_broadcast(b->cond);
    pthread_mutex_unlock(a->lock);
    pthread_cond_broadcast(a->cond);
}

static pthread_mutex_t read_lock = PTHREAD_MUTEX_INITIALIZER;

void *thread_main(void *input){
    char *buf = NULL;
    size_t cap = 0;
    if(input != NULL) {
        parse_and_execute(input);
        fflush(stdout);
    }

    while(true){
        // lock
        if(pthread_mutex_lock(&read_lock)) errExit("producer pthread mutex lock error");
        ssize_t result = getline(&buf, &cap, stdin);

        // unlock
        if(pthread_mutex_unlock(&read_lock)) errExit("producer pthread mutex unlock error");
        if(result >= 0)
            parse_and_execute(buf);
        else break;
    }
    return NULL;
}