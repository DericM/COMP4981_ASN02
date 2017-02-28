#ifndef SERVER_H
#define SERVER_H

void server();
int connect_server();
int readRequest(struct myMesg *buf, int msqid);
int readFile(struct myMesg *buf);
int openFile(char *fileName);


#endif