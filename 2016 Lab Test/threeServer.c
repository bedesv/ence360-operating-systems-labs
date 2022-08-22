#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

int server_socket(char *port) 
// execute socket(), bind(), listen(), accept() and return the new socket from accept()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;          /* communicate using internet address */
    sa.sin_addr.s_addr = INADDR_ANY; /* accept all calls                   */
    sa.sin_port = htons(atoi(port)); /* this is port number                */

    bind(s, (struct sockaddr *)&sa, sizeof(sa)); /* bind address to socket   */

    listen(s, 5); /* listen for connections on a socket */

    struct sockaddr_in client_address;
    /////////////////////////////////////////////
    socklen_t socklen = sizeof(struct sockaddr_in);
    int client_socket =  accept(s, (struct sockaddr *) &client_address, &socklen);

    /////////////////////////////////////////////

    return client_socket;
}


int main(int argc, char *argv[])  {

    if (argc != 2) { //if user doesn't provide the clients hostname and port number 
            fprintf(stderr,"usage: threeServer port-number\n"); 
            exit(1); 
    } 

	printf("\nThis is the server with pid %d listening on port %s\n", getpid(), argv[1]);
	
	int sockfd = server_socket(argv[1]);
	
	char* message = "congrats you successfully connected to the server!\n";
	write (sockfd, message, strlen(message));

    close(sockfd);

    exit (0);
}

