/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: client.cpp - An application to receive requests from clients and 
--	send back designated files using a message queue.
--
-- PROGRAM: asn02
--
-- FUNCTIONS:
-- void client();
-- void response_listener(struct myMesg *buf, int msqid);
-- int mesg_input(struct myMesg *buf);
--
-- DATE: Febuary 16, 2017
--
-- DESIGNER: Deric Mccadden
-- PROGRAMMER: Deric Mccadden
--
-- NOTES:
-- The program will get a file name from the user, it will then transfer it via the message queue
-- to the server. The server will respond with the file contents and the client will print them to the screen.
----------------------------------------------------------------------------------------------------------------------*/
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

#include <iostream>
#include <fstream>

#include <istream>
#include <string>

#include "mesg.h"
#include "common.h"
#include "client.h"

int running = 1;


/*--------------------------------------------------------------------------
-- FUNCTION: The main client loop. This contains the logic for the client.
-- void client()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
void client(){
	struct myMesg buf;
	int msqid;

    msqid = connect_client();
    buf.mtype = 1;

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



/*--------------------------------------------------------------------------
-- FUNCTION: This opens a message queue for the client and connects to it. 
-- int connect_client()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int connect_client(){
	int msqid;
	key_t key = generate_key();
	if ((msqid = msgget(key, 0644)) == -1) { 
        perror("msgget");
        exit(1);
    }
    return msqid;
}



/*--------------------------------------------------------------------------
-- FUNCTION: This listens for responses from the server.
-- int response_listener()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
void response_listener(struct myMesg *buf, int msqid){
	while(running){
		mesg_recv(buf, msqid, getpid());
    	printf("Server: Results... \n\n%s\n\n", buf->mtext);
	}
}



/*--------------------------------------------------------------------------
-- FUNCTION: This gathers input from the user.
-- int response_listener()
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int mesg_input(struct myMesg *buf){
	printf("client: Enter file name or 'exit': ");

	std::string temp = std::to_string(getpid());
	std::string temp2 = " ";
	std::string temp3;
	std::cin >> temp3;

	temp.append(temp2);
	temp.append(temp3);  

	temp.copy(buf->mtext, temp.length()+1);

    return 0;
}



