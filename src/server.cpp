#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <dirent.h> 

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <signal.h>

#include <unistd.h>



#include "asn02.h"


void server() {
	struct myMesg buf;
	int msqid;

	//std::thread exitThread(exit_listener, &buf, msqid);
    //exitThread.detach();

    msqid = connect_server();
    
    printf("server: Ready to receive requests.\n");
    while(1) {
        mesg_recv(&buf, msqid, 0);
        
        if(fork() == 0){
        	printf("server: Request for file \"%s\" from process %lu.\n",
        									buf.mtext, buf.mtype);
        	readFile(&buf, buf.mtext);
        	mesg_send(&buf, msqid);
        	return;
        }
    }	
    mesg_close(msqid);
    return;
}


/*
void exit_listener(struct myMesg *buf, int msqid){
	std::st
	while(running){
		
	}
}*/



int connect_server(){
	int msqid;
	key_t key = generate_key();
	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    return msqid;
}



int readFile(struct myMesg *buf, char *fileName) {
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
		   return 0;
		}
		else{
			strncpy(buf->mtext, buffer, sizeof(buf->mtext) - 1); 
      		buf->mtext[sizeof(buf->mtext) - 1] = '\0';
		}

		fclose(handler);
	} 
	else{
		printf("Did not find file '%s' in '%s'\n", fileName, FILE_DIR);
		strcpy(buffer, "File not found");
		strncpy(buf->mtext, buffer, sizeof(buf->mtext) - 1); 
      		buf->mtext[sizeof(buf->mtext) - 1] = '\0';
		return 0;
	}
	return 1;
}



