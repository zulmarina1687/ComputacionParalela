#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define BARRIER_COUNT 100

int thread_count;
int barrier_thread_counts[BARRIER_COUNT];
pthread_mutex_t barrier_mutex;

void Usage(char* prog_name);
void *Thread_work(void* rank);

int main(int argc, char* argv[]){
	long       thread, i;
	pthread_t* thread_handles; 
	clock_t start, finish;
	double total_t;

	if (argc != 2)
		Usage(argv[0]);
	thread_count = strtol(argv[1], NULL, 10);

	thread_handles = malloc (thread_count*sizeof(pthread_t));
	for (i = 0; i < BARRIER_COUNT; i++)
		barrier_thread_counts[i] = 0;
	pthread_mutex_init(&barrier_mutex, NULL);
	
	start = clock();
	for (thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, Thread_work, (void*) thread);
	for (thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handles[thread], NULL);
	}
	finish = clock();
	total_t = (finish - start) / CLOCKS_PER_SEC;
	printf("Time = %f segundos\n", total_t);

	pthread_mutex_destroy(&barrier_mutex);
	free(thread_handles);
	return 0;
}
void Usage(char* prog_name){
	fprintf(stderr, "usage: %s <# threads>\n", prog_name);
	exit(0);
}
void *Thread_work(void* rank){
	#ifdef DEBUG
	long my_rank = (long) rank; 
	#endif
	int i;
	for (i = 0; i < BARRIER_COUNT; i++){
		pthread_mutex_lock(&barrier_mutex);
		barrier_thread_counts[i]++;
		pthread_mutex_unlock(&barrier_mutex);
		while (barrier_thread_counts[i] < thread_count);
		#ifdef DEBUG
		if (my_rank == 0){
			printf("todos los hilos entraron en la barrera %d\n", i);
			fflush(stdout);
		}
		#endif
	}
	return NULL;
}