#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <pthread.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <thread>

#include "asn02.h"

int running = 1;

void client(){
	struct myMesg buf;
	int msqid;

    msqid = connect_client();
    buf.mtype = getpid();

    std::thread readThread(response_listener, &buf, msqid);
    readThread.detach();

    while(running) { 
    	mesg_input(&buf);
    	if(strcmp(buf.mtext, "exit") == 0) {
    		running = 0;
    		break;
    	}
    	mesg_send(&buf, msqid);
    }
    return;
}


int connect_client(){
	int msqid;
	key_t key = generate_key();
	if ((msqid = msgget(key, 0644)) == -1) { 
        perror("msgget");
        exit(1);
    }
    return msqid;
}



void response_listener(struct myMesg *buf, int msqid){
	while(running){
		mesg_recv(buf, msqid, getpid());
    	printf("Server: Results... \n\n%s\n\n", buf->mtext);
	}
}





int mesg_input(struct myMesg *buf){
	printf("client: Enter file name or 'exit': ");
	if(fgets(buf->mtext, sizeof(buf->mtext), stdin) == NULL) {
		return -1;
	}
	int len = strlen(buf->mtext);
    if (buf->mtext[len-1] == '\n'){
    	buf->mtext[len-1] = '\0';
    }

    return 0;
}



