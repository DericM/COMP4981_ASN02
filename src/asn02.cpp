/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: asn02.cpp - This is a client server combination. Using the commandline arguments 
--  'c' and 's' the user is able to determine which respective application he would like to launch.
--
-- PROGRAM: asn02
--
-- FUNCTIONS:
-- int main (int argc, char *argv[])
--
-- DATE: Febuary 16, 2017
--
-- DESIGNER: Deric Mccadden
-- PROGRAMMER: Deric Mccadden
----------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "mesg.h"
#include "common.h"
#include "client.h"
#include "server.h"


/*--------------------------------------------------------------------------
-- FUNCTION: Entrypoint to the program, selects client or server to run.
-- int main (int argc, char *argv[])
--
-- DATE: Febuary 16, 2017
--
-- AUTHOR: Deric Mccadden
---------------------------------------------------------------------------*/
int main (int argc, char *argv[]) {

	if(argc < 2 || (argv[1][0] != 's' && argv[1][0] != 'c') ){
		printf ("Usage: %s s (server)\n", argv[0]);
		printf ("Usage: %s c (client)\n", argv[0]);
		return -1;
	}
	
	switch(argv[1][0]){
		case 's':
			server();
            break;
		case 'c':
			client();
            break;
		default:
            break;
	}
    printf("Exiting process: %d\n", getpid());
   	return 0;
}

