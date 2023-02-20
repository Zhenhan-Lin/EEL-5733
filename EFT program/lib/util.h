//
// Created by 林真含 on 2023/2/9.
//

#ifndef EEL5733_UTIL_H
#define EEL5733_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <stdbool.h>
#define errExit(...){ \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);              \
}

char match_regex(const char *regex_pattern, char *input) {
    regex_t regex;
    int reti;
    int result;

    // compile regular expression
    reti = regcomp(&regex, regex_pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return -1;
    }

    // execute regular expression
    reti = regexec(&regex, input, 0, NULL, 0);
    if (!reti) {
        result = true;
    } else if (reti == REG_NOMATCH) {
        result = false;
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        return -1;
    }
    regfree(&regex);
    return result;
}

#endif //EEL5733_UTIL_H
