#include <stdio.h>
#include <mpi.h>

void Get_Input(int* n, double* a, double* b, int my_rank, int comm_sz);
double Trap(double left_endpt,double right_endpt,int trap_count,double base_len);
double f(double x);

int main(void){
	int my_rank, comm_sz, n = 1024, local_n;
	double a = 0.0, b = 3.0, h, local_a, local_b;
	double local_int, total_int;
	int source;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	Get_Input(&n,&a,&b,my_rank,comm_sz);
	
	h = (b-a)/n;
	local_n = n/comm_sz;
	local_a = a + my_rank*local_n*h;
	local_b = local_a + local_n * h;
	local_int = Trap(local_a,local_b,local_n,h);

	if(my_rank != 0){
		MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}else{
		total_int = local_int;
		for (source = 1;source < comm_sz;source ++){
			MPI_Recv(&local_int,1,MPI_DOUBLE, source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}
	if(my_rank == 0){
		printf("Con n = %d trapezoides, nuestra estimación \n", n);
		printf("de la integral de %f a %f = %.15e\n",a,b,total_int);
	}
	MPI_Finalize();
	return 0;
}

void Get_Input(
	int* n_p 	/*out*/,
	double* a_p	/*out*/,
	double* b_p	/*out*/,
	int my_rank /*in*/,
	int comm_sz /*in*/){
	int dest;

	if(my_rank == 0){
		printf("Ingrese n, a, y b\n");
		scanf("%d %lf %lf", n_p, a_p, b_p);
		for(dest = 1; dest < comm_sz; dest++){
			MPI_Send(n_p,1,MPI_INT,dest,0,MPI_COMM_WORLD);
			MPI_Send(a_p,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
			MPI_Send(b_p,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
		}
	}
	else{
		MPI_Recv(n_p,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(a_p,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(b_p,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
}

double Trap(
	double left_endpt	/*in*/,
	double right_endpt	/*in*/,
	int trap_count 		/*in*/,	
	double base_len		/*in*/){
	double estimate,x;
	int i;

	estimate = (f(left_endpt) + f(right_endpt))/2.0;
	for(i = 1; i <= trap_count-1; i ++){
		x = left_endpt + i*base_len;
		estimate += f(x);
	}
	estimate = estimate*base_len;
	return estimate;
}

double f(double x){
	return 2 * x + 1;
}