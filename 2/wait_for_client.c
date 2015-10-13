#include "wait_for_client.h"
#include <netinet/in.h>
#include <stdio.h>

/* Block the caller until a message is received on sfd,
 * and connect the socket to the source addresse of the received message.
 * @sfd: a file descriptor to a bound socket but not yet connected
 * @return: 0 in case of success, -1 otherwise
 * @POST: This call is idempotent, it does not 'consume' the data of the message,
 * and could be repeated several times blocking only at the first call.
 */
int wait_for_client(int sfd)
{
    struct sockaddr_in6 address;
    socklen_t addressL;
    int r,c;
    r=recvfrom(sfd,NULL,0,0,(struct sockaddr *)&address,&addressL);
    c=connect(sfd,(struct sockaddr *)&address,addressL);
    if(r==-1 || c==-1)
        return -1;
    return 0;
}


