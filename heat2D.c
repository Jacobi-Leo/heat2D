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

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	size1 = X / p;
	size2 = X -(p-1)*size1;
	if ( my_rank != n-1 )
		size = size1;
	else
		size = size2;

	double u_local[size+2][Y];
	double tmp[size+2][Y];
	tag = 0;
	if ( my_rank == 0 ) {
		inidat( X, Y, u_global );
		memcpy( &u_local[0][0], &u_global[0][0], sizeof(double)*size*Y );
		for ( int i=1; i<p; i++ ) {
			if ( i != p-1 )
				MPI_Send( u_global+i*size1*Y, size1*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
			else
				MPI_Send( u_global+i*size1*Y, size2*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
		}
	}
	else
		MPI_Recv( u_local+Y, size*Y, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );

	for ( tag=1; tag<ITER+1; tag++ ) {
		for ( int i=0; i<p; i++ ) {
			if ( i == 0 ) {
				//blabla
			}
			else if ( i == p-1 ) {
				//blabla
			}
			else {
				//blabla
			}
		}
	}
	
	if ( my_rank == 0 ) {
		memcpy( &u_global[0][0], &u_local[0][0], sizeof(double)*size*Y );
		for ( int i=1; i<p; i++ ) {
			if ( i != p-1 )
				MPI_Recv( u_global+i*size1*Y, size1*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status );
			else
				MPI_Recv( u_global+i*size1*Y, size2*Y, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status );
		}
		prtdat(X, Y, u_global);
	}
	else
		MPI_Send( u_local+Y, size*Y, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );
	
	MPI_Finalize();
	return 0;
}
