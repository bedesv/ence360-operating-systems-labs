/* Title: waiting.c
 * Description: ENCE360 Example code - Signals.
 */

#include <stdlib.h>
#include <unistd.h>     /* defines fork() */
#include <signal.h>


int NUM_QUITS = 0;


void sigquit() {
    signal(SIGQUIT, sigquit);
    write(1, "SIGQUIT\n", 8);
    if (NUM_QUITS == 0) {
        NUM_QUITS++;
    } else {
        exit(0);
    }
}

void install_handler() {
    signal(SIGQUIT, sigquit);
}

int main(int argc, const char * argv[]) {
    install_handler();
    for (; ; sleep(1)); /*loop forever*/
}

