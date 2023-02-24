//
// Created by 林真含 on 2023/2/19.
//

#ifndef EEL5733_ACCOUNT_H
#define EEL5733_ACCOUNT_H
#include <stdio.h>
#include <pthread.h>
typedef struct Account {
    size_t number;
    long balance;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
}* Account;

extern Account acc_list;

#endif //EEL5733_ACCOUNT_H
