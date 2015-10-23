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
#include "real_address.h"
#include "read_write_loop.h"
#include "create_socket.h"



/*--------------------------------------------------------------------------------------------------*/

/*                            définition des variables globales				  														*/

/*--------------------------------------------------------------------------------------------------*/

char* filename=NULL;

char* hostname;

int port=-1;



struct timeval timeBuf [16];

//pkt_t buffer [16];

int currentSeq; //Actuel numéro de séquence envoyé

int lastSeq; //Dernier numéro de séquence envoyé confirmé







/*--------------------------------------------------------------------------------------------------*/

/*                                   définition de fonctions																				*/

/*--------------------------------------------------------------------------------------------------*/

void* producer(); //lit le fichier et met les paquets dans le buf

void* sender(); //envoie les paquets

//void* listen(); //ecoute les ACK (et renvoie si besoin)

void* checkTimer(); //verifie que le Timer le plus ancien ne depasse pas le RTT







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
	int socket = create_socket(NULL,0,&addr,port);
	if(socket==-1)
	{
		fprintf(stderr,"Create Socket error\n");
		close(socket);
		return -1;
	}
	read_write_loop(socket);
	close(socket);

	return 0;

}
