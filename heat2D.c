#include<stdio.h>
#include<mpi.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define X 1024
#define Y 1024
#define ITER 500
#define CX 0.000002
#define CY 0.000002

int main(int *argc, char **argv) {
    int size, size1, size2, tag, p, my_rank;
    double u_global[X][Y];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    size1 = X / p;
    size2 = X -(p-1)*size1;
    if ( my_rank != n-1 )
	size = size1;
    else
	size = size2;

    double u_local[2][size+2][Y];
    int z = 0;
    tag = 0;
    if ( my_rank == 0 ) {
	inidat( X, Y, u_global );
	memcpy( &u_local[z][0][0], &u_global[0][0], sizeof(double)*size*Y );
	for ( int i=1; i<p; i++ ) {
	}
    }
}
