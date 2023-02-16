//
// Created by 林真含 on 2023/2/9.
//

#include "lib/thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/calendar_filter_t.c"
#include "inc/email_filter_t.c"
#include "lib/queue.h"
#include "lib/util.h"

int main(int argc, char *argv[]){
    size_t MAXSIZE = atoi(argv[1]);

    // Thread identifiers
	pthread_t tid[2];
	// Initialize queue
	InitQueue(MAXSIZE);
	// Create the threads; may be any number, in general
	if (pthread_create(&tid[0], NULL, email_filter, NULL))
		errExit("Unable to create producer\n");
	if (pthread_create(&tid[1], NULL, calendar_filter, NULL))
		errExit("Unable to create consumer\n");
	// Wait for created thread to exit
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

    return 0;
}