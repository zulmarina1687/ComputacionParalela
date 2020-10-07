#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>

#define BARRIER_COUNT 100

int thread_count;
int counter;
sem_t barrier_sems[BARRIER_COUNT];
sem_t count_sem;

void Usage(char* prog_name);
void *Thread_work(void* rank);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread, i;
   pthread_t* thread_handles; 
   clock_t start, finish;
   double total_t;
   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   for (i = 0; i < BARRIER_COUNT; i++)
      sem_init(&barrier_sems[i], 0, 0);
   sem_init(&count_sem, 0, 1);

   start = clock();
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL, Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }
   finish = clock();
   total_t = (finish - start) / CLOCKS_PER_SEC;
   printf("Time = %f segundos\n", total_t);

   sem_destroy(&count_sem);
   for (i = 0; i < BARRIER_COUNT; i++)
      sem_destroy(&barrier_sems[i]);
   free(thread_handles);
   return 0;
}
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
}
void *Thread_work(void* rank){
   #ifdef DEBUG
   long my_rank = (long) rank;
   #endif
   int i, j;
   for(i = 0; i < BARRIER_COUNT; i++){
      sem_wait(&count_sem);
      if (counter == thread_count - 1) {
         counter = 0;
         sem_post(&count_sem);
         for (j = 0; j < thread_count-1; j++)
            sem_post(&barrier_sems[i]);
      } else {
         counter++;
         sem_post(&count_sem);
         sem_wait(&barrier_sems[i]);
      }
      #ifdef DEBUG
      if (my_rank == 0) {
         printf("All threads completed barrier %d\n", i);
         fflush(stdout);
      }
      #endif
   }
   return NULL;
}