#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <sys/stat.h>
#include <sys/types.h>

#include <mqueue.h>

#include "asn02.h"

int main (int argc, char *argv[])
{

	if(argc < 2 || (argv[1][0] != 's' && argv[1][0] != 'c') ){
		printf ("Usage: %s s (server)\n", argv[0]);
		printf ("Usage: %s c (client)\n", argv[0]);
		return -1;
	}
	
	switch(argv[1][0]){
		case 's':
			server();
            break;
		case 'c':
			client();
            break;
		default:
            break;
	}

   	return 0; 
}





int mesg_send(mqd_t mq, char * buffer){

    // send the message 
    mq_send(mq, buffer, MAX_SIZE, 0);

    return 0;
}



int mesg_recv(mqd_t mq, char * buffer){

    ssize_t bytes_read;
    int result = 0;

    /* receive the message */
    bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
    if(bytes_read <= 0){
        return result;
    }

    buffer[bytes_read] = '\0';

    if (! strncmp(buffer, MSG_STOP, strlen(MSG_STOP)))
    {
        result = 1;
    }
    else
    {
        printf("Received: %s\n", buffer);
    }

    return result;
}








