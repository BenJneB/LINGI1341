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

struct timeval timeBuf [16];
pkt_t buffer [16];
int currentSeq; //Actuel numéro de séquence envoyé
int lastSeq; //Dernier numéro de séquence envoyé confirmé



/*--------------------------------------------------------------------------------------------------*/
/*                                   définition de fonctions																				*/
/*--------------------------------------------------------------------------------------------------*/
void* producer(); //lit le fichier et met les paquets dans le buf
void* sender(); //envoie les paquets
void* listen(); //ecoute les ACK (et renvoie si besoin)
void* checkTimer(); //verifie que le Timer le plus ancien ne depasse pas le RTT



int main(int argc, char **argv){
	if(argc == 2){ //Mode Entrée Standard
		int i = atoi (argv[1]+1);
		if (j == 0) {perror("Port is not a number");}
	}
	else if (argc ==4) {//Mode Fichier
	
	}
	else{
	perror("Number of Argument Invalid);	
	}
	return 0;
}
