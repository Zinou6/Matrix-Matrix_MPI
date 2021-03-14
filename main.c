#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"
#include "initMatrix.h"
#include "displayMatrix.h"
#include "calcul.h"

typedef struct block {
	int istart, iend;
	int taille;
}block;

MPI_Status status;
int main(int argc, char **argv) {
	int n;  							// MATRIX DIMENSION
	int i, j, k;							//Parcours
	double start_Time,end_Time;
	n = atoi (argv[1]);
	
	int nprocs, myrank, source, dest;	
	
	MPI_Init(&argc,&argv); 						//Initialisation MPI 
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	const int nitems = 3;
	int blocklenghts[3]={1,1,1};
	MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
	MPI_Datatype mpi_block_type;
	MPI_Aint offset[3];
	
	offset[0] = offsetof(block, istart);
	offset[1] = offsetof(block, iend);
	offset[2] = offsetof(block, taille);
	
	MPI_Type_create_struct(nitems, blocklenghts, offset, types, &mpi_block_type);
	MPI_Type_commit(&mpi_block_type);
	
	
	int **A = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++)
		A[i] = (int *)malloc(n * sizeof(int));
	generateRandomMatrix(A, n, 0, 98);
	
	int **B = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++)
		B[i] = (int *)malloc(n * sizeof(int));
	generateRandomMatrix(B, n, 0, 99);
	
	int **Result = (int **)malloc(n * sizeof(int *)); 
	for (i = 0; i < n; i++)
		Result[i] = (int *)malloc(n * sizeof(int));
		
	block bl;
	block bl2;
	bl.taille = n/nprocs;
	bl2.taille = n/nprocs;
	
	if(myrank == 0) {
		printf("\nje suis le proc %d de %d \n",myrank,nprocs);
		
		printf("Matrix size = %d\n", n);
		printMatrix(A, n);
		printf("\n");
		printMatrix(B, n);
		printf("\n");			
		
		start_Time = MPI_Wtime();
		
		if(nprocs == 1) {
			MultMatrices(A,B,Result,n);
			end_Time = MPI_Wtime();
			printMatrix(Result , n);
			printf("\n Execution time on %2d nodes : %f \n",nprocs,end_Time-start_Time);		
		}else{	
			bl.istart = myrank * bl.taille;
			bl.iend   = bl.istart + bl.taille -1;
			for(i = bl.istart; i<=bl.iend; i++)
				for(j = bl.istart; j<bl.iend; j++)
					Result[i][j] = 0;
					for(int k = 0; k < n; ++k){
						Result[i][j] += A[i][k] * B[k][j];	
					}
			if(myrank != nprocs-1){
				dest = myrank+1;
			}else{ dest = 0; }
			if(myrank != 0){
				source = myrank-1;
			}else{ source = nprocs-1; }
			
			MPI_Send(&bl, 1, mpi_block_type, dest, 1, MPI_COMM_WORLD);
			MPI_Recv(&bl2, 1, mpi_block_type, source, 1, MPI_COMM_WORLD,&status);
			printMatrix(Result , n);
			}
			
		}else{
			printf("\nje suis le proc  %d de %d \n",myrank,nprocs);
			bl2.istart = myrank * bl2.taille;
			bl2.iend   = bl2.istart + bl2.taille -1;
			for(i = bl.istart; i<=bl.iend; i++)
				for(j = bl.istart; j<bl.iend; j++)
					Result[i][j] = 0;
					for(int k = 0; k < n; ++k){
						Result[i][j] += A[i][k] * B[k][j];	
					}
			if(myrank != nprocs-1){
				dest = myrank + 1;
			}
			
			source = myrank -1;
			MPI_Send(&bl2, 1, mpi_block_type, dest, 1, MPI_COMM_WORLD);
			MPI_Recv(&bl2, 1, mpi_block_type, source, 1, MPI_COMM_WORLD,&status);
			printMatrix(Result , n);
		}
		
		if(myrank = nprocs-1){
			printf("\n\n");
    			printMatrix(Result, n);
    			printf("\n\n");
		}
	
	
	MPI_Finalize();
	
	return 0;
	
}
