#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

#include "asn02.h"


int sendFileName(mqd_t mq, char * buffer);

void client(){
	
	mqd_t mq_server;
	mqd_t mq_clients;

    char buffer[MAX_SIZE];

    mq_server  = mq_open(QUEUE_SERVER, O_WRONLY);
    mq_clients = mq_open(QUEUE_CLIENTS, O_RDONLY);


    if(!mq_server || !mq_clients){
    	printf("Could not open message queues.");
    }


    printf("Send to server (enter \"exit\" to stop it):\n");

    printf("> ");


    while(strncmp(buffer, MSG_STOP, strlen(MSG_STOP))){
    	fflush(stdout);

        char buffer[MAX_SIZE];
    
	    memset(buffer, 0, MAX_SIZE);
	    fgets(buffer, MAX_SIZE, stdin);

	    
	    sendFileName(mq_server, buffer);


	    mesg_recv(mq_server, buffer);
        char * fileContents = readFile(buffer);
	    if(fileContents == NULL){
	    	
	    }
	    else{
	    	puts(fileContents);
	    }



    }

    mq_close(mq_server);
	
}





int sendFileName(mqd_t mq, char * buffer){

	mesg_send(mq, buffer);
	return 0;
}



