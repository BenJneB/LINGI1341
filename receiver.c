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
#include <packet_implem.h>

/*--------------------------------------------------------------------------------------------------*/
/*                            définition des variables globales				  														*/
/*--------------------------------------------------------------------------------------------------*/
char* filename=NULL;
char* hostname;
int port;

pkt_t buffer [16];
int currentSeq; //le plus recent numéro de seq recu
int oldSeq; //le plus ancien numéro de seq e nattente



/*--------------------------------------------------------------------------------------------------*/
/*                                   définition de fonctions																				*/
/*--------------------------------------------------------------------------------------------------*/
void* consumer(); //prend les paquets du buf pour creer new fichier
void* checkPKT(); //regarde les paquets recus (met dans buffer si OK sinon envoie ACK)


int main(int argc, char **argv){
	if(argc < 2 || argc > 5){
	perror("Number of arguments invalid");
	}
	else{
	//
	}
	return 0;
}
