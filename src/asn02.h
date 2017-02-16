#ifndef ASN02_H
#define ASN02_H

#define FILE_DIR   "files/"
#define MAX_SIZE   1024

#define MAXMESSAGEDATA (4096-16)
#define MESGHDRSIZE (sizeof(Mesg) - MAXMESSAGEDATA)

#include <sys/types.h>



struct myMesg {
	long mtype;
    char mtext[MAXMESSAGEDATA];
};


//common
key_t generate_key();
int connect_server();
int connect_client();
int mesg_send(struct myMesg *buf, int msqid);
int mesg_recv(struct myMesg *buf, int msqid, long msgtype);
void mesg_close(int msqid);

//client
void client();
void response_listener(struct myMesg *buf, int msqid);
int mesg_input(struct myMesg *buf);

//server
void server();
int readFile(struct myMesg *buf, char *fileName);


#endif