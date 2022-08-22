#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>

#define MAXDATASIZE 1024  


int main(int argc, char *argv[]) 
{ 
    if (argc != 3) {  // Not the right number of args error. Print message and exit
            fprintf(stderr,"usage: threeClient hostname port-number\n"); 
            exit(1); 
    }

    // Initialise socket using socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo their_addrinfo; // server address info
	struct addrinfo *their_addr = NULL; // connector's address information
	memset(&their_addrinfo, 0, sizeof(struct addrinfo)); // Initialise memory for address info to 0
	their_addrinfo.ai_family = AF_INET;        /* communicate using internet address */
	their_addrinfo.ai_socktype = SOCK_STREAM;  /* use TCP rather than datagram */
	getaddrinfo(argv[1], argv[2], &their_addrinfo, &their_addr); /* get IP address info for the hostname (argv[1]) and port (argv[2]) */

    // Connect to server using connect
	connect(sockfd, their_addr->ai_addr, their_addr->ai_addrlen);

	char buffer[MAXDATASIZE]; //buffer contains data from/to server
	int numbytes; // number of bytes of data read from socket
	// get data from the server:
    numbytes = read(sockfd, buffer, MAXDATASIZE-1); // receive data from the host/server

	while (numbytes > 0) // While the message is not empty
	{
		buffer[numbytes-1] = '\0'; // Remove the last character of the received string
		printf("%s\n", buffer); //print out what you received 

		// send data to the server and then get data back from the server:
        write(sockfd, buffer, strlen(buffer)); // Send data to the server
		numbytes = read(sockfd, buffer, MAXDATASIZE-1); // receive data from the server
	}

    // Close the connection to the server
	close(sockfd);

    return 0; 
} 



/* The expected output is listed below - very briefly describe why the output appears like this:

Because when the server or the client receive a message, they replace the last character
 with a null terminator, shortening the string by one. Since they both do it, the string
 has been shortened by two characters every time it's printed by the client

congrats you successfully connected to the server
congrats you successfully connected to the serv
congrats you successfully connected to the se
congrats you successfully connected to the
congrats you successfully connected to th
congrats you successfully connected to
congrats you successfully connected t
congrats you successfully connected
congrats you successfully connect
congrats you successfully conne
congrats you successfully con
congrats you successfully c
congrats you successfully
congrats you successful
congrats you successf
congrats you succes
congrats you succ
congrats you su
congrats you
congrats yo
congrats
congrat
congr
con
c

*/

