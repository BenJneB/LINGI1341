#include "real_address.h"

#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

const char * real_address(const char * address, struct sockaddr_in6 * rval) {

    struct addrinfo hints;
    struct addrinfo *result;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = IPPROTO_UDP;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s=getaddrinfo(address,NULL, &hints,&result);
    if(s != 0)
                return(gai_strerror(s));

    struct sockaddr_in6 * addr = (struct sockaddr_in6 *) result->ai_addr;
    memcpy(rval, addr, sizeof(struct sockaddr_in6));


    freeaddrinfo(result);

	return(NULL);

}
