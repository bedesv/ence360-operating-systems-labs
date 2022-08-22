#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // initialised to unlocked state

int global_data = 0;

void read_data();
void set_data();

int main()
{
	pthread_t thread1, thread2; // Initialise thread identifiers
    int thread_return1, thread_return2; // Initialise variables for the return value of creating the threads
	int i;

    // Loop five times
    // Each time create two threads. One that sets the data and one that reads the data.
    for(i = 0; i < 5; i++) {
		thread_return1 = pthread_create( &thread1, NULL, (void*)&set_data, NULL);
     	thread_return2 = pthread_create( &thread2, NULL, (void*)&read_data, NULL);
	}
    
	// wait for threads to finish before continuing
    // Only have to wait for the two most recent threads as due to the mutex, they
    // will finish after the previously created threads.
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("exiting\n");
    exit(0);
}

void set_data()
{
	// protect critical region:
	pthread_mutex_lock(&mutex1);

    // Set the data to a new value
	printf("Setting data\t");
	global_data = rand();

    // Unlock the critical region
    pthread_mutex_unlock(&mutex1);
}

void read_data()
{
	int data;

	// protect critical region:
    pthread_mutex_lock(&mutex1);

    // Print out the current data value
	data = global_data;
	printf("Data: %d\n", data);

    // Unlock the critical region
    pthread_mutex_unlock(&mutex1);
}


/* Run the program and write down the results displayed:
Setting data	Data: 1804289383
Setting data	Data: 846930886
Setting data	Data: 1681692777
Setting data	Data: 1714636915
Setting data	Data: 1957747793
exiting
*/
