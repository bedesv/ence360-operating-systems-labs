#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2000

int listen_on(int port)
{

    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;          /* communicate using internet address */
    sa.sin_addr.s_addr = INADDR_ANY; /* accept all calls                   */
    sa.sin_port = htons(port); /* this is port number                */

    int rc = bind(s, (struct sockaddr *)&sa, sizeof(sa)); /* bind address to socket   */
    if (rc == -1) { // Check for errors
        perror("bind");
        exit(1);
    }

    rc = listen(s, 5); /* listen for connections on a socket */
    if (rc == -1) { // Check for errors
        perror("listen");
        exit(1);
    }

    return s;
}


int accept_connection(int s) {

    struct sockaddr_in client_address;
    /////////////////////////////////////////////
    // TODO: Implement in terms of 'accept'
    socklen_t socklen = sizeof(struct sockaddr_in);
    int client_socket =  accept(s, (struct sockaddr *) &client_address, &socklen);

    /////////////////////////////////////////////  

    return client_socket;
}


void handle_request(int msgsock) {
    ///////////////////

      // This initial code reads a single message (and ignores it!)
    char buffer[MAXDATASIZE];
    int num_read = 0;

//    //read a message from the client
//    num_read = read(msgsock, buffer, MAXDATASIZE - 1);
//    printf("read a message %d bytes: %s\n", num_read, buffer);


    // TODO: write a function to reply to all incoming messages
    // while the connection remains open
    do {
        memset(buffer, 0, MAXDATASIZE);
        if ((num_read = read(msgsock, buffer, MAXDATASIZE - 1)) < 0) {
            perror("network server read");
            close(msgsock);
            exit(7);
        } else if (num_read == 0) { // Close if receives 0 bytes
            printf("\nServer closing client connection\n");
            close(msgsock);
            continue; /* Break out of the loop */
        } else { // respond to the message from the client
            buffer[num_read] = '\0';
            printf("read a message %d bytes: %s\n", num_read, buffer);
            write(msgsock, buffer, num_read);
        }

    } while (num_read != 0);

    ///////////////////


}


// handle request by forking a new process
void handle_fork(int msgsock) {

    //TODO: run this line inside a forked child process
    if (fork() == 0) {
        handle_request(msgsock);
    } else {
        close(msgsock);
    }


    // Be very careful to close all sockets used, 
    // and exit any processes or threads which aren't used
      // Note that sockets open BEFORE a fork() are open in BOTH parent/child

    ///////////////////////////////////////////
}





int main(int argc, char *argv[]) {


    printf("\nThis is the server with pid %d listening on port %d\n", getpid(), SERVER_PORT);

    // setup the server to bind and listen on a port
    int s = listen_on(SERVER_PORT);

    while (1) { // forever

        int msgsock = accept_connection(s); // wait for a client to connect
        printf("Got connection from client!");

        // handle the request with a new process
        handle_fork(msgsock);
    }

    close(s);
    exit(0);
}

