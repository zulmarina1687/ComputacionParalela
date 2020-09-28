#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){
	MPI_Init(&argc, &argv);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status stat;

	if(size != 2){
		if(rank == 0){
			printf("Este programa requiere exactamente 2 rangos de MPI, pero está intentando usar %d! Exiting...\n", size);
		}
		MPI_Finalize();
		exit(0);
	}
	for(int i=0; i<=27; i++){
		long int N = 1 << i;
   	 	//Asignar memoria para A en la CPU
		double *A = (double*)malloc(N*sizeof(double));
		//Inicializar todos los elementos de A a 0.0
		for(int i=0; i<N; i++){
			A[i] = 0.0;
		}
		int tag1 = 10;
		int tag2 = 20;
		int loop_count = 50;
		//Bucle de levantamiento
		for(int i=1; i<=5; i++){
			if(rank == 0){
				MPI_Send(A, N, MPI_DOUBLE, 1, tag1, MPI_COMM_WORLD);
				MPI_Recv(A, N, MPI_DOUBLE, 1, tag2, MPI_COMM_WORLD, &stat);
			}
			else if(rank == 1){
				MPI_Recv(A, N, MPI_DOUBLE, 0, tag1, MPI_COMM_WORLD, &stat);
				MPI_Send(A, N, MPI_DOUBLE, 0, tag2, MPI_COMM_WORLD);
			}
		}
		//Tiempo de ping-pong para iteraciones loop_count de tamaño de transferencia de datos 8*N bytes
		double start_time, stop_time, elapsed_time;
		start_time = MPI_Wtime();
		for(int i=1; i<=loop_count; i++){
			if(rank == 0){
				MPI_Send(A, N, MPI_DOUBLE, 1, tag1, MPI_COMM_WORLD);
				MPI_Recv(A, N, MPI_DOUBLE, 1, tag2, MPI_COMM_WORLD, &stat);
			}
			else if(rank == 1){
				MPI_Recv(A, N, MPI_DOUBLE, 0, tag1, MPI_COMM_WORLD, &stat);
				MPI_Send(A, N, MPI_DOUBLE, 0, tag2, MPI_COMM_WORLD);
			}
		}
		stop_time = MPI_Wtime();
		elapsed_time = stop_time - start_time;
		long int num_B = 8*N;
		long int B_in_GB = 1 << 30;
		double num_GB = (double)num_B / (double)B_in_GB;
		double avg_time_per_transfer = elapsed_time / (2.0*(double)loop_count);
		if(rank == 0) printf("Size (B): %10li, Tiempo (s): %15.9f, Ancho Banda (GB/s): %15.9f\n", num_B, avg_time_per_transfer, num_GB/avg_time_per_transfer );
		free(A);
	}
	MPI_Finalize();
	return 0;
}