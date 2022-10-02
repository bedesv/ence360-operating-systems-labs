//
// Created by bjs185 on 3/10/22.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int x;

    MPI_Comm_size(MPI_COMM_WORLD, &x);

    printf("%d\n", x);

}