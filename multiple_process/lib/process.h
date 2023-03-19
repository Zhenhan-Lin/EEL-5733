//
// Created by 林真含 on 2023/2/10.
//

#ifndef EEL5733_THREAD_H
#define EEL5733_THREAD_H
#include <stdio.h>
#include <pthread.h>

void process_init();
// The function thread_puts() writes the string s, and a terminating newline character, to the stream stdout.
void process_puts(const char *);
// Get the string from thread_puts
const char * process_getline();

#endif //EEL5733_THREAD_H
