//
// Created by 林真含 on 2023/2/9.
//

#ifndef EEL5733_UTIL_H
#define EEL5733_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#define errExit(...){ \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);              \
}

#endif //EEL5733_UTIL_H
