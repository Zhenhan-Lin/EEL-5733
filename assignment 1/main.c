//
//  main.c
//  assignment_1
//
//  Created by 林真含 on 2023/1/25.
//

#include <stdio.h>
#include <regex.h>
#include <string.h>

struct event{
    char date[10];
    char time[5];
    char location[10];
    int number;
}event[100];

//char event_lookup(){
//    return 0;
//}

int main(){
    char input[100];
    char operation;
    int i = 0;

//C,Meeting   ,01/12/2019,15:30,NEB202
    while (fgets(input, sizeof(input), stdin) != NULL) {
        operation = input[0];
        memcpy(event[i].date, input+13,10);
        memcpy(event[i].time, input+24,5);
        memcpy(event[i].location,input+30,10);
        switch(operation){
            case 'C': printf("%.10s,%.5s,%.10s\n", event[i].date, event[i].time, event[i].location); break;
            case 'X': printf("%.10s,%.5s,%.10s\n", event[i].date, event[i].time, event[i].location); break;
            case 'D': printf("%.10s,--:--,NA\n", event[i].date); break;
            default:{
                printf("OPERATION NOT FOUND");
                return -1;
            }
        }
        i += 1;
    }
}

