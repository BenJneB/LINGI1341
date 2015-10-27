#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include "packet_interface.h"
#include "real_address.h"
#include "read_write_loop.h"
#include "create_socket.h"



/*--------------------------------------------------------------------------------------------------*/

/*                            définition des variables globales				  														*/

/*--------------------------------------------------------------------------------------------------*/

char* file=NULL;

char* hostname;

int port=-1;

char *buf;

struct timeval timeBuf [16];

//pkt_t *window[32];
/*int i=0;
while(i<32)
{
	window[i]=NULL;
	i++;
}*/

int currentSeq; //Actuel numéro de séquence envoyé

int lastSeq; //Dernier numéro de séquence envoyé confirmé







/*--------------------------------------------------------------------------------------------------*/

/*                                   définition de fonctions																				*/

/*--------------------------------------------------------------------------------------------------*/

void* producer(); //lit le fichier et met les paquets dans le buf

//void* sender(); //envoie les paquets

//void* listen(); //ecoute les ACK (et renvoie si besoin)

void* checkTimer(); //verifie que le Timer le plus ancien ne depasse pas le RTT

int writeSocket(int socket, size_t length,char *buffer)
{
	ssize_t writed=write(socket,(void *)buffer,length);
	if(writed==-1)
	{
		fprintf(stderr,"Error in writeSocket\n");
		return -1;
	}	
	if(writed==EOF)
		printf("EOFwrite\n");	
	return writed;
}

int readSocket(int socket, char *buffer, int length)
{
	ssize_t readed=read(socket,(void *)buffer,length);
	if(readed==-1)
	{
		fprintf(stderr,"Error in readSocket\n");
		return -1;
	}
	if(readed==EOF)
		printf("EOFread\n");		
	return readed;
}

int readFile(int f, char *buffer,int length)
{
	ssize_t readed=read(f,(void *)buffer,length);
	if(readed==-1)
	{
		fprintf(stderr,"Error in readFile\n");
		return -1;
	}
	if(readed==EOF)
		printf("EOFreadfile\n");
	return readed;
}
void sender(int socket,char *filename)
{
	//pkt_t *pkt=pkt_new();
	int f;
	printf("start sender\n");
	if(filename==NULL)
		f=STDIN_FILENO;
	else
		f=open(filename,O_RDONLY);
	printf("afteropen\n");
	fd_set read;//, write;
	
	//FD_ZERO(&write);
	char readed[512];
while(1){

	//FD_SET(socket,&write);
FD_ZERO(&read);
	FD_SET(f,&read);
	FD_SET(socket,&read);
	
	printf("beforeselect\n");
	int s=select(socket+1,&read,NULL,NULL,NULL);
	printf("aftersel\n");
	if(s==-1)
	{
		fprintf(stderr,"Error selectsender\n");
		return;
	}
printf("boucle\n");
	if(FD_ISSET(socket,&read))
	{
		printf("Sender Read Socket\n");
		int rd=readSocket(socket,readed,4);//ACK OR NACK size ? 4 or 8
		if(rd==0)
		{
			pkt_t *pkt=pkt_new();
			pkt_status_code errdec=pkt_decode((const char *) readed,4,pkt);
			if(errdec==PKT_OK)
			{
				ptypes_t type=pkt_get_type(pkt);
				if(type==PTYPE_ACK)
				{
					printf("ACK\n");
				}

				else if(type==PTYPE_NACK)
				{
					printf("NACK\n");
				}
				else
				{
					fprintf(stderr,"Sender receive wrong type\n");
					return;
				}
			}
			pkt_del(pkt);		
		}
		
	}
	else if(FD_ISSET(f,&read))
	{
		printf("Sender read file\n");
		int rf=readFile(f,readed,512);
		char *bufn=malloc(sizeof(char)*512);
		strcpy(bufn,readed);
		printf("hahaha %s\n",bufn);
		if(rf>=0)
		{
			pkt_t *pkt=pkt_new();
			pkt_set_type(pkt,PTYPE_DATA);
			pkt_set_length(pkt,rf);
			pkt_set_window(pkt,0);
			pkt_set_seqnum(pkt,0);
			pkt_set_payload(pkt,readed,(uint16_t)rf);
			//win
			//seqnum
			printf("rf:%d\n",rf);
			char tab[520];
			size_t l=520;
			pkt_status_code errenc=pkt_encode(pkt,tab,(size_t *)&l);
			printf("l=%d\n",l);
			if(errenc==PKT_OK)
			{
				if(writeSocket(socket,l,tab)==-1)
				{
					fprintf(stderr,"Error sender writesocket\n");
					return;
				}
			}
			else
			{
				fprintf(stderr,"Error sender encode\n");
				return;
			}
			
			pkt_del(pkt);
		}
	
	}

}
} 
//faut un while qqpart




int main(int argc, char **argv){

	hostname=malloc(50*sizeof(char));
	buf=malloc(520*sizeof(char));

    	if(argc < 3)

    	{

        	fprintf(stderr, "Invalid number of argument : Need hostname AND port\n");
		free(hostname);
		return -1;

    	}

    	int arg;

    	for(arg=1;arg<argc;arg++)

    	{

        	if(strcmp("-f",argv[arg])==0 && argc>=arg+1)

        	{

            		file=argv[arg+1];

            		arg++;

            		printf("%s\n",file);

        	}

        	else if(strcmp("--filename",argv[arg])==0 && argc>=arg+1)

        	{

           		file=argv[arg+1];

            		arg++;

            		printf("%s\n",file);

        	}

        	else

        	{

            		strcpy(hostname,argv[arg]);

            		printf("%s\n", hostname);
	    		if(argc>=arg+1)
			{

            			if(atoi(argv[arg+1])!=0)
					port=atoi(argv[arg+1]);

            			else
				{
					fprintf(stderr, "Invalid port number\n");
					free(hostname);
					return -1;
				}
				printf("%d\n",port);
		    		arg++;
			}
	    		else
			{
				fprintf(stderr,"Need a port number or a hostname\n");
				free(hostname);
				return -1;
			}

        	}



    	}
    	if(arg==3 && port==-1)

    	{

        	fprintf(stderr, "Invalid number of argument : Need hostname AND port\n");
		free(hostname);
		return -1;

    	}

	

	struct sockaddr_in6 addr;
	//addr=malloc(sizeof(addr));
	const char *erraddr=real_address(hostname,&addr);
	if(erraddr!=NULL)
	{
		fprintf(stderr,"%s\n",erraddr);
		return -1;
	}
	int socket = create_socket(NULL,-1,&addr,port);
	if(socket==-1)
	{
		fprintf(stderr,"Create Socket error\n");
		close(socket);
		return -1;
	}
	sender(socket,file);
	close(socket);

	return 0;

}
