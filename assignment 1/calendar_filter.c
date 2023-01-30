//
//  main.c
//  assignment_1
//
//  Created by 林真含 on 2023/1/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define earlier 1
#define later 2
#define same 3

#define delete (-2)
#define noprint 6
#define printout 7

#define to_num(str) (str-'0')
#define value_d(n1,n2) (10*to_num(n1)+to_num(n2))
#define value_y(n1,n2,n3,n4) (1000*to_num(n1)+100*to_num(n2)+10*to_num(n3)+to_num(n4))
#define print(eve) printf("%.10s,%.5s,%.10s\n", eve.date, eve.time, eve.location)

//TODO: X->Delete line

struct events{
    char date[10];
    char time[5];
    char location[10];
    bool if_print;
    int number;a
}event[100];

void struct_copy(struct events *dest, int ori_num){
    memcpy(dest->date, event[ori_num].date,10);
    memcpy(dest->time, event[ori_num].time,5);
    memcpy(dest->location,event[ori_num].location,10);
    dest->if_print = event[ori_num].if_print;
    dest->number = event[ori_num].number;
}

int date_compare(char times1[16], char times2[16]){
    int month = value_d(times1[0],times1[1]) - value_d(times2[0],times2[1]);
    int day = value_d(times1[3],times1[4]) - value_d(times2[3],times2[4]);
    long year = value_y(times1[6],times1[7],times1[8],times1[9]) - value_y(times2[6],times2[7],times2[8],times2[9]);
    int hour = value_d(times1[11],times1[12]) - value_d(times2[11],times2[12]);
    int minute = value_d(times1[14],times1[15]) - value_d(times2[14],times2[15]);

    if(year < 0) return earlier;
    else if(year > 0) return later;
    else{
        if(month < 0) return earlier;
        else if(month > 0) return later;
        else{
            if(day < 0) return earlier;
            else if(day > 0) return later;
            else{
                if(hour < 0) return earlier;
                else if(hour > 0) return later;
                else{
                    if(minute < 0) return earlier;
                    else if(minute > 0) return later;
                    else return same;
                }
            }
        }
    }
}

void earliest_check(struct events *earl, int num){
    char eve_time[16];
    char earl_time[16];

    for (int j=0;j<num;j++) {
        if(j==0){
            struct_copy(earl, j);
            memcpy(earl_time, earl[j].date, 15);
        }
        else{
            int re;
            memcpy(eve_time, eve_time, 15);
            re = date_compare(earl_time,eve_time);
            if(re == later){
                struct_copy(earl, j);
            }
        }
    }
}

int event_lookup(int num, char operation){
    char date_time[16];
    memcpy(date_time, event[num].date, 15);
    int result;
    int j;

    for(j=0;j<num;j++){
        char traverse_date_time[16];
        char tmp[16];
        memcpy(traverse_date_time, event[j].date, 15);
        result = date_compare(date_time, traverse_date_time);

        if(result == same){
            if(operation == 'D'){
                strcpy(tmp, "99/99/999999:99");
                memcpy(event[j].date, tmp, 10);
                memcpy(event[j].time, tmp+10,5);
                memcpy(event[j].location,event[num].location,10);

                struct events *earl;
                earl = (struct events*)malloc(sizeof(struct events));
                earliest_check(earl, num);
                if(earl->if_print == false) return earl->number;
                else return delete;
            }else if(operation == 'X'){
                memcpy(event[j].location, event[num].location,10);
                if(event[j].if_print == true) return printout;
                else return noprint;
            }
        }
    }
    return 0;
}


bool equal_check(char arr1[10], char arr2[10]){
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

int event_compare(int num){
    char date_time[16];
    char location[10];
    memcpy(date_time, event[num].date, 15);
    memcpy(location, event[num].location, 10);
    int result = earlier;
    int j;

    for(j=0;j<num;j++){
        char traverse_date_time[16];
        char traverse_location[10];
        memcpy(traverse_date_time, event[j].date, 15);
        memcpy(traverse_location, event[j].location, 10);
        result = date_compare(date_time, traverse_date_time);
        if(result == later) return result;
        else if(result == same){
           if(equal_check(location, traverse_location) == false){
               result = earlier;
           }
       }
    }

    return result;
}

int main(){
    char input[100];
    char operation;
    int i = 0;

    while (fgets(input, sizeof(input), stdin) != NULL) {
        operation = input[0];
        memcpy(event[i].date, input+13,10);
        memcpy(event[i].time, input+24,5);
        memcpy(event[i].location,input+30,10);
        event[i].if_print = false;
        event[i].number = i;

        int result;

        switch(operation){
            case 'C':
                //只有当事件较之前的某一事件更早，才会输出
                result = event_compare(i);
                if(result == earlier) {
                    print(event[i]);
                    event[i].if_print = true;
                }
                else if(result == later) break;
                break;
            case 'X':
                //输出当前事件以及更改之前的事件
                 if(event_lookup(i,'X') == printout){
                     print(event[i]);
                     event[i].if_print = true;
                 }
                break;
            case 'D':
                //输出特殊标识以及删除之前的事件
                result = event_lookup(i,'D');
                if(result == delete) printf("%.10s,--:--,NA\n", event[i].date);
                else{
                    print(event[result]);
                    event[result].if_print = true;
                }
                char tmp[16];
                strcpy(tmp, "99/99/999999:99");
                memcpy(event[i].date, tmp, 10);
                memcpy(event[i].time, tmp+10, 5);
                event[i].if_print = true;
                break;
            default:{
                printf("OPERATION NOT FOUND");
                return -1;
            }
        }
        i += 1;
    }
}

