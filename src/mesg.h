#ifndef MESG_H
#define MESG_H

#define FILE_DIR   "files/"
#define MAXMESSAGEDATA (4096-16)

struct myMesg {
	long mtype;
	char mtext[MAXMESSAGEDATA];
};


#endif