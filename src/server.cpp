/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: server.cpp - An application to receive requests from clients and 
--	send back designated files using a message queue.
--
-- PROGRAM: asn02
--
-- FUNCTIONS:
-- void server();
-- int readRequest(struct myMesg *buf, int msqid);
-- int readFile(struct myMesg *buf);
-- int openFile(char *fileName);
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

#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "mesg.h"
#include "common.h"
#include "server.h"

std::ifstream file;
std::string gfileName;




/*--------------------------------------------------------------------------
-- FUNCTION: The main server loop. This contains the logic for the server.
-- void server()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
void server() {
	struct myMesg buf;
	int msqid;
    msqid = connect_server();
    
    printf("server: Ready to receive requests.\n");
    while(1) {
    	buf.mtype = readRequest(&buf, msqid);
        
        if(fork() == 0){
        	printf("server: Request for file \"%s\" from process %lu.\n",
        									gfileName.c_str(), buf.mtype);
        	openFile((char *)gfileName.c_str());

        	while(readFile(&buf)){
        		mesg_send(&buf, msqid);
        	}
        	return;
        }
    }	
    mesg_close(msqid);
    return;
}




/*--------------------------------------------------------------------------
-- FUNCTION: This creates a message queue for the server and connects to it. 
-- int connect_server()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int connect_server(){
	int msqid;
	key_t key = generate_key();
	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    return msqid;
}



/*--------------------------------------------------------------------------
-- FUNCTION: Reads a requested filename from stdin. 
-- int readRequest(struct myMesg *buf, int msqid)
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int readRequest(struct myMesg *buf, int msqid){
	mesg_recv(buf, msqid, 1);
	std::string temp(buf->mtext);
	std::istringstream iss(temp);
	std::string mtype;
	iss >> mtype;
	iss >> gfileName;
	return std::stoi( mtype );
}


/*--------------------------------------------------------------------------
-- FUNCTION: Reads a preselected file in chunks, returns 0 when we reach the end. 
-- int readFile(struct myMesg *buf)
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int readFile(struct myMesg *buf) {
	if(file.eof()){
		return 0;
	}
	file.read(buf->mtext, sizeof(buf->mtext) - 1);
	return 1;
}


/*--------------------------------------------------------------------------
-- FUNCTION: Opens a file for reading. 
-- int openFile(char *fileName)
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int openFile(char *fileName) {
	strtok(fileName, "\n");
	char path[1024];
	snprintf(path, sizeof(path), "%s%s", FILE_DIR, fileName);
	file.open(path);
	if(!file.is_open()){
		return 0;
	}
	return 1;
}

