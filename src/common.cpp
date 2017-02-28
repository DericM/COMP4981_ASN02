/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: common.cpp - This contains functions shared between the client and the server.
--
-- PROGRAM: asn02
--
-- FUNCTIONS:
-- key_t generate_key();
-- int mesg_send(struct myMesg *buf, int msqid);
-- int mesg_recv(struct myMesg *buf, int msqid, long msgtype);
-- void mesg_close(int msqid);
--
-- DATE: Febuary 16, 2017
--
-- DESIGNER: Deric Mccadden
-- PROGRAMMER: Deric Mccadden
--
-- NOTES:
-- The program will create and monitor a message queue for incomming requests.
-- The requests will come appended with a pid.
-- The program will attempt to open the requested files.
-- The program will then attempt to send the contents of the files back on the message queue with
--		the priority specified by the pid.
----------------------------------------------------------------------------------------------------------------------*/
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

#include "mesg.h"
#include "common.h"


/*--------------------------------------------------------------------------
-- FUNCTION: Generates a key used in creating and identifying the message queue.
-- key_t generate_key()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
key_t generate_key(){
	key_t key;
    if ((key = ftok("src/server.cpp", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    return key;
}


/*--------------------------------------------------------------------------
-- FUNCTION: Send a message to the message queue.
-- int mesg_send(struct myMesg *buf, int msqid)
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int mesg_send(struct myMesg *buf, int msqid){
	//printf("send: msqid:%d mtype:%lu buf:%s\n", msqid, buf->mtype, buf->mtext);
    int len = strlen(buf->mtext);
    if (msgsnd(msqid, buf, len+1, 0) == -1) {
        perror("msgsnd");
    }
    return 0;
}


/*--------------------------------------------------------------------------
-- FUNCTION: Receive a message from the message queue.
-- int mesg_recv(struct myMesg *buf, int msqid, long msgtype)
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int mesg_recv(struct myMesg *buf, int msqid, long msgtype){
	//printf("recv: msqid:%d mtype:%lu buf:%s\n", msqid, buf->mtype, buf->mtext);
    if (msgrcv(msqid, buf, sizeof(buf->mtext), msgtype, 0) == -1) {
        perror("msgrcv");
        printf("%d", msqid);
        exit(1);
    }
    return 1;
}


/*--------------------------------------------------------------------------
-- FUNCTION: Close the message queue.
-- void mesg_close(int msqid)
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
void mesg_close(int msqid){
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
}

