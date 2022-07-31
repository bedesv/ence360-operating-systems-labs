#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int has_run[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void runMe(int *arg) {
    int value = (*arg);
    assert(value >= 0 && value < 5 && "Bad argument passed to 'runMe()!'");

    has_run[value] = 1;

    int *ret = (int*)malloc(sizeof(int));
    *ret = value * value;

    pthread_exit((void*)ret);
}

int run_threads(int n) {

    int total = 0;
    int nums[n];
    pthread_t pthreads[n];
    for (int i = 0; i < n; i++) {
        nums[i] = i;
        pthread_create(&pthreads[i], NULL, (void*) runMe, &nums[i]);
    }

    for (int i = 0; i < n; i++) {
        void *ret;
        pthread_join(pthreads[i], &ret);
        total += *(int*) ret;
        free(ret);
    }
    return total;
}

int main (int argc, char **argv) {

    int sum = run_threads(5);

    int correct = 0;
    for(int i = 0; i < 5; ++i) {
        if(has_run[i]) correct++;
    }

    printf("%d %d", correct, sum);

    return 0;
}

