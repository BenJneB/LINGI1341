#include "read_write_loop.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_SEGMENT_SIZE 1024

/*
 * Loop reading a socket and printing to stdout,
 * while reading stdin and writing to the socket
 * @sfd: The socket file descriptor. It is both bound and connected.
 * @return: as soon as stdin signals EOF
 */

void read_write_loop(int sfd) {

	fd_set readS,writeS;
	int readB=1;
	char buf[MAX_SEGMENT_SIZE];

	FD_ZERO(&readS);
	FD_ZERO(&writeS);


	while(true)
	{

    	if(readB)
    	{
        	FD_SET(STDIN_FILENO,&readS);
        	FD_SET(sfd,&writeS);
    	}
    	else
    	{
        	FD_SET(sfd,&readS);
        	FD_SET(STDOUT_FILENO,&writeS);
    	}
    	select(sfd+1,&readS,&writeS,NULL,NULL);

    	if(FD_ISSET(STDIN_FILENO,&readS) && FD_ISSET(sfd,&writeS))
    	{
        	ssize_t sizeR=read(STDIN_FILENO,buf,MAX_SEGMENT_SIZE);
        	if(sizeR==EOF) readB=0;

        	ssize_t sizeW=0;
        	while(sizeW != sizeR)
        	{
        	    ssize_t temp=write(sfd,buf,sizeR);
            	sizeW += temp;
        	}
    	}
    	if(FD_ISSET(sfd,&readS) && FD_ISSET(STDOUT_FILENO,&writeS) && !readB)
    	{
        	ssize_t sizeR=read(sfd,buf,MAX_SEGMENT_SIZE);
        	if(sizeR==EOF) readB=0;

        	ssize_t sizeW=0;
        	while(sizeW != sizeR)
        	{
        	    ssize_t temp=write(STDOUT_FILENO,buf,sizeR);
            	sizeW += temp;
        	}
    	}


	}
}
