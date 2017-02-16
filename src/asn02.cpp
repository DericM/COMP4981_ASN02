#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "asn02.h"



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

