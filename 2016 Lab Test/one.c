#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_function( void *ptr );

int main()
{
    pthread_t childPid; // create an identifier for the child thread
    //  create the child thread and tell it to run print_message_function
	pthread_create(&childPid, NULL, print_message_function, NULL);
	sleep(1); // wait for one second
	return(0); // Exit the main thread
}

void *print_message_function( void *ptr )
{
	printf("child thread\n");
	printf("Hello world\n");
    sleep(5); // wait for 5 seconds
    printf("after 5 seconds\n");
	pthread_exit(NULL); // Exit the child thread
}
