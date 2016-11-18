#include<stdio.h>
#include<mpi.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include"data.h"
#define X 2000
#define Y 2000
#define ITER 5000
/*#define DEBUG*/

int main(int argc, char **argv) {
    int size, size1, size2, tag, p, my_rank;
    double *u_global;
    double start_time, end_time;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    size1 = X / p;
    size2 = X -(p-1)*size1;
    if ( my_rank != p-1 ) {
        size = size1;
    }
    else {
        size = size2;
    }
#ifdef DEBUG
    printf("size of processor %d is %d, and size1 is %d, size2 is %d.\n", my_rank, size, size1, size2);
#endif

#ifdef DEBUG
    printf("I'm here in %d, at %d-th iteration.\n", my_rank, tag);
#endif
    double u_local[size+2][Y];
    double tmp[size+2][Y];
    /* Varialble Explanation:
     *
     * *tag* is used to mark the step of iterations and at the same time,
     * to control synchronization between different processors.
     */
    tag = 0;
    // distribute calculation data to different processors, and processor_0
    // holds the global data.
    if ( my_rank == 0 ) {
        /*u_global = new double[X*Y]*/
        u_global = (double *)malloc(X*Y*sizeof(double));
#ifdef DEBUG
        if ( u_global == NULL ) {
            printf("failed to build u_global\n");
        }
#endif
        inidat( X, Y, u_global );
        prtdat( X, Y, u_global, "init.txt" );
        start_time = MPI_Wtime();
        memcpy( &u_local[0][0], u_global, sizeof(double)*size*Y );
        for ( int i=1; i<p; i++ ) {
            if ( i != p-1 )
                MPI_Send( u_global+i*size1*Y, size1*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
            else
                MPI_Send( u_global+i*size1*Y, size2*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
        }
        free(u_global);
    }
    else {
        MPI_Recv( &u_local[1][0], size*Y, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status );
    }
    // END of initial data distribution

    for ( tag=1; tag<ITER; tag++ ) {
        // boudary conditions are sent to each processor, and then
        // calculation is processed.
#ifdef DEBUG
        printf("I'm here in %d, at %d-th iteration.\n", my_rank, tag);
#endif
        if ( my_rank == 0 ) {
            MPI_Send( &u_local[size-1][0], Y, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD );
            MPI_Recv( &u_local[size][0], Y, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, &status );
            update( size+1, Y, &u_local[0][0], &tmp[0][0] );
            memcpy( &u_local[0][0], &tmp[0][0], sizeof(double)*Y*(size+1) );
        }
        else if ( my_rank == p-1 ) {
            MPI_Send( &u_local[1][0], Y, MPI_DOUBLE, p-2, tag, MPI_COMM_WORLD );
            MPI_Recv( &u_local[0][0], Y, MPI_DOUBLE, p-2, tag, MPI_COMM_WORLD, &status );
            update( size+1, Y, &u_local[0][0], &tmp[0][0] );
            memcpy( &u_local[0][0], &tmp[0][0], sizeof(double)*Y*(size+1) );
        }
        else {
            MPI_Send( &u_local[1][0], Y, MPI_DOUBLE, my_rank-1, tag, MPI_COMM_WORLD );
            MPI_Send( &u_local[size][0], Y, MPI_DOUBLE, my_rank+1, tag, MPI_COMM_WORLD );
            MPI_Recv( &u_local[0][0], Y, MPI_DOUBLE, my_rank-1, tag, MPI_COMM_WORLD, &status );
            MPI_Recv( &u_local[size+1][0], Y, MPI_DOUBLE, my_rank+1, tag, MPI_COMM_WORLD, &status );
            update( size+2, Y, &u_local[0][0], &tmp[0][0] );
            memcpy( &u_local[0][0], &tmp[0][0], sizeof(double)*Y*(size+2) );
        }
    }

    // reduce data into  u_global
    u_global = (double *)malloc(X*Y*sizeof(double));
    if ( my_rank == 0 ) { // the 0th proc receives data
        memcpy( u_global, &u_local[0][0], sizeof(double)*size*Y );
        for ( int i=1; i<p; i++ ) {
            if ( i != p-1 )
                MPI_Recv( u_global+i*size1*Y, size1*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status );
            else
                MPI_Recv( u_global+i*size1*Y, size2*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status );
        }
        end_time = MPI_Wtime();
        printf( "time to run the program is %f\n", end_time-start_time );
        prtdat(X, Y, u_global, "result.txt");
    }
    else { // the 1st to (p-1)th proc send data to the 0th proc
        MPI_Send( &u_local[1][0], size*Y, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );
    }
#ifndef DEBUG
#endif
    MPI_Finalize();
    return 0;
}
