

#define MAXMESSAGEDATA (4096-16) /* don't want sizeof(Mesg) > 4096 */
#define MESGHDRSIZE (sizeof(Mesg) - MAXMESSAGEDATA)


typedef struct
{
	int  mesg_len; 
	long mesg_type;
	char mesg_data [MAXMESSAGEDATA];
} Mesg;




