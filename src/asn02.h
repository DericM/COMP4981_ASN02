#ifndef ASN02_H
#define ASN02_H


/*--------- globals ----------------*/

#define QUEUE_SERVER  "/server"
#define QUEUE_CLIENTS "/clients"

#define MSG_STOP   "exit"
#define FILE_DIR   "files/"

#define MAX_SIZE   1024


void server();
void client();

int mesg_send(mqd_t mq, char * buffer);
int mesg_recv(mqd_t mq, char * buffer);

char* readFile(char *fileName);


#endif