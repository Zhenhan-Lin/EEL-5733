//
//  calendar_filter.c
//  assignment_1
//
//  Created by Zhenhan Lin on 2023/1/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../lib/thread.h"
#include "util.h"

#define earlier 1
#define later 2
#define same 3


#define to_num(str) (str-'0')
#define value_d(n1,n2) (10*to_num(n1)+to_num(n2))
#define print(eve) \
    printf("%.10s,%.5s,%.10s\n", eve.date, eve.time, eve.location);  \
    eve.if_print = true

int event_days = 0;
int n[100] = {0, 0};  // [0]: exist number of dates, [1]: the number of events of No. i

//memcmp
bool equal_check(char arr1[], char arr2[]){
    bool equal = true;
    int count = 0;
    while (count < 10)
    {
        if (arr1[count] != arr2[count]) {
            equal = false;
            break;
        }
        count++;
    }
    return equal;
}

struct events{
    char title[10];
    char date[10];
    char time[5];
    char location[10];
    bool if_print;
    int number[2];
}event[100][100];

char printed_date[100][10];

void event_refresh(int day, int num){
    for (int j = 0; j < n[day]; j++) {
        if(j != num)
            event[day][j].if_print = false;
    }
}

void struct_copy(struct events *dest, int ori_day, int ori_num){
    memcpy(dest->title, event[ori_day][ori_num].title,10);
    memcpy(dest->date, event[ori_day][ori_num].date,10);
    memcpy(dest->time, event[ori_day][ori_num].time,5);
    memcpy(dest->location,event[ori_day][ori_num].location,10);
    dest->number[0] = event[ori_day][ori_num].number[0];
    dest->number[1] = event[ori_day][ori_num].number[1];
}

int date_compare(char times1[5], char times2[5]){
    int hour = value_d(times1[0],times1[1]) - value_d(times2[0],times2[1]);
    int minute = value_d(times1[3],times1[4]) - value_d(times2[3],times2[4]);

    if(hour < 0) return earlier;
    else if(hour > 0) return later;
    else{
        if(minute < 0) return earlier;
        else if(minute > 0) return later;
        else return same;
    }
}

void earliest_check(struct events *earl, int day, int num){
    char eve_time[5];
    char earl_time[5];

    for (int j=0;j<n[day];j++) {
        if(j==0){
            struct_copy(earl, day, j);
            earl->if_print = event[day][j].if_print;
            memcpy(earl_time, earl->time, 5);
        }
        else{
            int re;
            memcpy(eve_time, event[day][j].time, 5);
            re = date_compare(earl_time,eve_time);
            if(re == later){
                struct_copy(earl, day, j);
                earl->if_print = event[day][j].if_print;
                memcpy(earl_time, earl->time, 5);
            }
        }
    }
}

void delete_event(int d, int num) {
    char tmp[16], tmpl[11];
    strcpy(tmp, "99/99/999999:99");
    strcpy(tmpl, "XXXXXXXXXX");
    memcpy(event[d][num].title, tmpl, 10);
    memcpy(event[d][num].date, tmp, 10);
    memcpy(event[d][num].time, tmp+10, 5);
    event[d][num].if_print = true;
}

void event_lookup_X(int day, int num){
    char title[10];
    struct events *earl;

    memcpy(title, event[day][num].title, 10);
    earl = (struct events*)malloc(sizeof(struct events));

    for(int d=0;d<event_days;d++) {
        for (int j = 0; j < n[d]; j++) {
            char traverse_title[10];
            memcpy(traverse_title, event[d][j].title, 10);

            if (equal_check(title, traverse_title) == true) {   // find the original event, abstract delete
                if(d != day || j != num) {
                    delete_event(d, j);
                    // when change date to another day, need to check if the earliest event of the original day changed
                    if (d != day) {
                        earliest_check(earl, d, n[d]);

                        char tmp[11];
                        memcpy(tmp, earl->date, 10);
                        if (equal_check(tmp, "99/99/9999") == true)
                            printf("%.10s,--:--,NA\n", printed_date[d]);
                        else if (earl->if_print == false) {
                            print(event[earl->number[0]][earl->number[1]]);
                            event_refresh(earl->number[0], earl->number[1]);
                        }
                    }
                    goto GOTO;
                }
            }
        }
    }

GOTO:
    earliest_check(earl, day, num);
    //if the current event is the earliest one in the day
    if(equal_check(earl->title, title)){
        print(event[day][num]);
        event_refresh(day, num);
//        if_print_check(day, num);
    }
    //if the earliest event changed
    else if(earl->if_print == false){
        print(event[earl->number[0]][earl->number[1]]);
        event_refresh(earl->number[0], earl->number[1]);
//        if_print_check(day, num);
    }
}

void event_lookup_D(int day, int num){
    char info[25];
    struct events *earl;

    memcpy(info, event[day][num].title, 25);
    earl = (struct events*)malloc(sizeof(struct events));

    for(int d=0;d<event_days;d++) {
        for (int j = 0; j < n[d]; j++) {
            char traverse_info[25];
            memcpy(traverse_info, event[d][j].title, 25);

            if (equal_check(info, traverse_info) == true) {   // find the original event, abstract delete
                if(j != num) {
                    delete_event(d, j);
                    delete_event(day, num);

                    earliest_check(earl, d, n[d]);

                    char tmp[11];
                    memcpy(tmp, earl->date, 10);
                    if (equal_check(tmp, "99/99/9999") == true)
                        printf("%.10s,--:--,NA\n", printed_date[d]);
                    else if (earl->if_print == false) {
                        print(event[earl->number[0]][earl->number[1]]);
                        event_refresh(earl->number[0], earl->number[1]);
                    }
                    return;
                }
            }
        }
    }
}

int event_compare(int day, int num){
    char date_time[5];
    char location[10];
    memcpy(date_time, event[day][num].time, 5);
    memcpy(location, event[day][num].location, 10);
    int result = earlier;
    int j;

    for(j=0;j<num;j++){
        char traverse_date_time[5];
        char traverse_location[10];
        memcpy(traverse_date_time, event[day][j].time, 5);
        memcpy(traverse_location, event[day][j].location, 10);
        result = date_compare(date_time, traverse_date_time);
        if(result == later) return result;
        else if(result == same){
           if(equal_check(location, traverse_location) == false){
               // same time but different location, print out the latter one
               result = earlier;
           }
       }
    }
    return result;
}

void * calendar_filter(void * argv){
    const char *input;
    char operation;
    int k;   //number of current event date

    while ((input = thread_getline()) != NULL) {
        operation = input[0];
        char tmp[10];
        bool found = false;
        memcpy(tmp, input+13,10);
        for(k=0; k<event_days; k++){
            if(equal_check(printed_date[k], tmp)){    // the date has been recorded
                found = true;
//                n[k]++;
                break;
            }
        }
        if(found==false){
            event_days++;
            memcpy(printed_date[k], input+13,10);
        }
        memcpy(event[k][n[k]].title, input+2,10);
        memcpy(event[k][n[k]].date, input+13,10);
        memcpy(event[k][n[k]].time, input+24,5);
        memcpy(event[k][n[k]].location,input+30,10);
        event[k][n[k]].number[0] = k;
        event[k][n[k]].number[1] = n[k];
        event[k][n[k]].if_print = false;

        n[k] += 1;
        int result;

        switch(operation){
            case 'C':
                // print out only when creating the earlier event that day
                result = event_compare(k, n[k]-1);
                if(result == earlier){
                    print(event[k][n[k]-1]);
                    event_refresh(k, n[k]-1);
                }
                break;
            case 'X':
                // record the changed event
                // print out when the earliest event of the day changed
                event_lookup_X(k, n[k]-1);
                break;
            case 'D':
                // delete the event
                // print out when the earliest event of the day changed
                event_lookup_D(k, n[k]-1);
                break;
            default:{
                errExit("OPERATION NOT FOUND");
            }
        }
        free((void *)input);
    }
    return NULL;
}

