#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

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

#include "create_socket.h"

#include "read_write_loop.h"

#include "real_address.h"

#include "wait_for_client.h"



#define MAXPKTSIZE 520

#define MAXSEQNUM 255

/*--------------------------------------------------------------------------------------------------*/

/*                            définition des variables globales				  														*/

/*--------------------------------------------------------------------------------------------------*/

char* filename=NULL;

char* hostname;

int port=-1;

pkt_t *packet;



//pkt_t buffer [16];

int currentSeq; //le plus recent numéro de seq recu

int oldSeq; //le plus ancien numéro de seq e nattente







/*--------------------------------------------------------------------------------------------------*/

/*                                   définition de fonctions																				*/

/*--------------------------------------------------------------------------------------------------*/

void* consumer(); //prend les paquets du buf pour creer new fichier

void* checkPKT(); //regarde les paquets recus (met dans buffer si OK sinon envoie ACK)





int main(int argc, char **argv){

	hostname=malloc(50*sizeof(char));

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

            		filename=argv[arg+1];

            		arg++;

            		printf("%s\n",filename);

        	}

        	else if(strcmp("--filename",argv[arg])==0 && argc>=arg+1)

        	{

           		filename=argv[arg+1];

            		arg++;

            		printf("%s\n",filename);

        	}

        	else

        	{

            		strcpy(hostname,argv[arg]);

            		printf("%s\n", hostname);
	    		if(argc>=arg+2)
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
	int socket = create_socket(&addr,port,NULL,0);
	if(socket==-1)
	{
		fprintf(stderr,"Create Socket error\n");
		close(socket);
		return -1;
	}
	printf("beforewait\n");
	int wait=wait_for_client(socket);
	if(wait==-1)
	{
		fprintf(stderr,"Impossible to connect after first reception\n");
		close(socket);
		return -1;
	}
	printf("afterwait\n");
	read_write_loop(socket);
	close(socket);

	return 0;

}