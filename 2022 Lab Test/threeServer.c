#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 1024  


int main(int argc, char *argv[])  {

    if (argc != 2) {  // Wrong number of args, print message and exit
            fprintf(stderr,"usage: threeServer port-number\n"); 
            exit(1); 
    } 

	printf("\nThis is the server with pid %d listening on port %s\n", getpid(), argv[1]);

    int s = socket(AF_INET, SOCK_STREAM, 0); // Create socket and get file descriptor of that socket

	struct sockaddr_in sa, caller; // Initialise socket info structs
	sa.sin_family = AF_INET;          /* communicate using internet address */
	sa.sin_addr.s_addr = INADDR_ANY; /* accept all calls                   */
	sa.sin_port = htons(atoi(argv[1])); /* this is port number                */

    bind(s, (struct sockaddr *)&sa, sizeof(sa)); /* bind address to socket   */

    listen(s, 5); /* listen for connections on a socket */

	socklen_t length = sizeof(caller);
    // Accept connection from a client
    int client_socket =  accept(s, (struct sockaddr *) &caller, &length);

	char message[MAXDATASIZE] = "congrats you successfully connected to the server!";
	while (strlen(message) > 0) // While the message is not empty
	{
		int numbytes; // number of bytes of data read from socket

		// send data to the client and then get data back from the client:
		write(client_socket, message, strlen(message)); // send data to the client
		numbytes = read(client_socket, message, MAXDATASIZE-1); // receive data from the client

		message[numbytes - 1] = '\0'; // Remove the last character of the received string
	}
    // Close the connection to the client
	close(client_socket);

    exit (0);
}

