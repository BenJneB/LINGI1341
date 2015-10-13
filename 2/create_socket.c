#include "create_socket.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/*
 * Creates a socket and initialize it
 * @source_addr: if !NULL, the source address that should be bound to this socket
 * @src_port: if >0, the port on which the socket is listening
 * @dest_addr: if !NULL, the destination address to which the socket should send
 * data
 * @return: a file descriptor number representing the socket,
 * or -1 in case of error (explanation will be printed on stderr)
 */


int create_socket(struct sockaddr_in6 *source_addr, int src_port, struct sockaddr_in6 *dest_addr, int dst_port) {

    int sfd=socket(AF_INET6,SOCK_DGRAM,IPPROTO_UDP);

    if(sfd==-1)
    {
        perror("error creation socket in createsocket");

        return -1;
    }

    if(source_addr !=NULL)
    {
        if(src_port > 0 ) source_addr->sin6_port=src_port;

        if(bind(sfd,(struct sockaddr *)source_addr,sizeof(struct sockaddr_in6))==-1)
        {
                perror("error bind in createsocket");
                return -1;
        }

    }

    if(dest_addr!=NULL)
    {
        if(dst_port>0) dest_addr->sin6_port=dst_port;

        if(connect(sfd,(struct sockaddr *)dest_addr,sizeof(struct sockaddr_in6))==-1)
        {
            perror("error connect in createsocket");
            return -1;
        }
    }

    return sfd;
}
