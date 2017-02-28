#ifndef CLIENT_H
#define CLIENT_H

void client();
int connect_client();
void response_listener(struct myMesg *buf, int msqid);
int mesg_input(struct myMesg *buf);


#endif


