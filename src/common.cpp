#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "asn02.h"


key_t generate_key(){
	key_t key;
    if ((key = ftok("src/server.cpp", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    return key;
}








int mesg_send(struct myMesg *buf, int msqid){
    int len = strlen(buf->mtext);
    if (msgsnd(msqid, buf, len+1, 0) == -1) {
        perror("msgsnd");
    }
    return 0;
}



int mesg_recv(struct myMesg *buf, int msqid, long msgtype){
    if (msgrcv(msqid, buf, sizeof(buf->mtext), msgtype, 0) == -1) {
        perror("msgrcv");
        printf("%d", msqid);
        exit(1);
    }
    return 1;
}



void mesg_close(int msqid){
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
}

