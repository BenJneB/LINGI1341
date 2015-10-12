#include "read_write_loop.h"

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024

/*
 * Loop reading a socket and printing to stdout,
 * while reading stdin and writing to the socket
 * @sfd: The socket file descriptor. It is both bound and connected.
 * @return: as soon as stdin signals EOF
 */

void read_write_loop(int sfd) {

struct pollfd pfd[3];
int eof=1;
int noerror;
pfd[0].fd=sfd;
pfd[1].fd=STDIN_FILENO;
pfd[2].fd=STDOUT_FILENO;
pfd[0].events= POLLIN | POLLOUT;
pfd[1].events=POLLIN;
pfd[2].events=POLLOUT;

while(eof)
{
    int i;
    //a finir
}


}
