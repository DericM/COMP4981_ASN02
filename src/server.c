#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <dirent.h> 

#include "asn02.h"






void server() {

    mqd_t mq_server;
    mqd_t mq_clients;



    struct mq_attr attr;

    char buffer[MAX_SIZE + 1];
    int must_stop = 0;

    // initialize the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // create the message queue
    mq_server  = mq_open(QUEUE_SERVER,  O_CREAT | O_RDONLY, 0644, &attr);
    mq_clients = mq_open(QUEUE_CLIENTS, O_CREAT | O_WRONLY, 0644, &attr);


    if(!mq_server || !mq_clients){
    	printf("Could not open message queues.");
    }

    
        
    

    while (!must_stop) {

        mesg_recv(mq_server, buffer);

        char * fileContents = readFile(buffer);
	    if(fileContents == NULL){
	    	mesg_send(mq_clients, "no such file");
	    }
	    else{
	    	mesg_send(mq_clients, fileContents);
	    	//puts(fileContents);
	    }

    }

    mq_close(mq_server);
    mq_close(mq_clients);
    mq_unlink(QUEUE_SERVER);
    mq_unlink(QUEUE_CLIENTS);

}




char* readFile(char *fileName) {

	strtok(fileName, "\n");

	char path[1024];
	snprintf(path, sizeof(path), "%s%s", FILE_DIR, fileName);


	char *buffer = NULL;
	int string_size, read_size;
	FILE *handler = fopen(path, "r");

	if (handler)
	{
		fseek(handler, 0, SEEK_END);
		string_size = ftell(handler);
		rewind(handler);

		buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
		read_size = fread(buffer, sizeof(char), string_size, handler);
		buffer[string_size] = '\0';

		if (string_size != read_size)
		{
		   free(buffer);
		   buffer = NULL;
		}

		fclose(handler);
	} 
	else{
		printf("Did not find '%s' in '%s'\n", fileName, FILE_DIR);
	}

	return buffer;
}







