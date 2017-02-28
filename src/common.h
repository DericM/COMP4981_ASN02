#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

key_t generate_key();
int mesg_send(struct myMesg *buf, int msqid);
int mesg_recv(struct myMesg *buf, int msqid, long msgtype);
void mesg_close(int msqid);


#endif