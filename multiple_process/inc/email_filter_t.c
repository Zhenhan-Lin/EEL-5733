//
// Created by 林真含 on 2023/2/9.
//
//
//  email_filter.c
//  assignment1
//
//  Created by Zhenhan Lin on 2023/1/19.
//

#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "../lib/util.h"

#define THREAD_PRINTF(...){ \
            char * puts_buf = malloc(256*sizeof(char)); \
            sprintf(puts_buf, __VA_ARGS__);  \
            process_puts(puts_buf); \
        }

#define True 1
#define False 0



struct calender{
    char operation;
    char title[10];
    char date[10];
    char time[5];
    char location[10];
}cal[100];

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
        result = True;
    } else if (reti == REG_NOMATCH) {
        result = False;
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        return -1;
    }
    regfree(&regex);
    return result;
}

void * email_filter(void * argv) {
    char *str = NULL;
    size_t len = 0;
    // input pattern
    char *regex_pattern = "Subject: [A-Z],([A-Za-z ]{10}),([0-9]{2}/[0-9]{2}/[0-9]{4}),([0-9]{2}:[0-9]{2}),([A-Za-z0-9 ]{10})";    // no validation
    int i = 0;
    int re;

    while (getline(&str, &len, stdin) >= 0) {
        re = match_regex(regex_pattern, str);
        if(re == True){
            cal[i].operation = str[9];
            memcpy(cal[i].title, str+11,10);
            memcpy(cal[i].date, str+22,10);
            memcpy(cal[i].time, str+33,5);
            memcpy(cal[i].location,str+39,10);
            THREAD_PRINTF("%c,%.10s,%.10s,%.5s,%.10s\n", cal[i].operation, cal[i].title, cal[i].date, cal[i].time, cal[i].location);
        }
        i += 1;
    }
    process_puts(NULL);
    return NULL;
}
