#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#define BUF_SIZE 1000

sem_t    full;
sem_t    empty;
sem_t    mutex;

int counter = 0;

int* arr;
int n;


void writer(){
	int i;
	int wr_idx=0;
	for (i=0; i<n; i++){	
		//decrement the empty semaphore
		wait(&empty);
		wait(&mutex);
		//wiriting to the queue
		arr[wr_idx] = i;
		wr_idx = (wr_idx+1)%BUF_SIZE;
		//increment the full semaphore
		signal(&mutex);
		signal(&full);
	}
}
void *reader(){
	int i;
	int read_idx = 0;
	int read_value;
	for(i=0; i<n; i++){
		//decrement the full semaphore
		wait(&full);
		wait(&mutex);
		//read item from the buffer
		arr[read_idx] = i;
		read_value = arr[read_idx];
		read_idx = (read_idx+1)%BUF_SIZE;
		//increments the empty semaphore
		signal(&mutex);
		signal(&empty);
		if(read_value!=i){
         		printf("Error: read value (%d) is out of sequence. It should be %d\n", read_value, i);
      }

	}
	struct timespec thread_time;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &thread_time);
	printf("\nThe CPU time consumed by the reader thread is %f\n", thread_time.tv_sec+thread_time.tv_nsec*1e-9);

}

void writer2(){
   int i;
   int wr_idx = 0;
   
   /* write n entries */
   for(i=0;i<n;i++){
      /* wait till queue is not full */
      while(counter==BUF_SIZE);
     
      /* write to queue */
      arr[wr_idx]=i;
      wr_idx = (wr_idx+1)%BUF_SIZE;

      /* increment the counter */
      sem_wait(&mutex);
      counter ++;
      sem_post(&mutex);
      
   }

}

void *reader2(){
   int i;
   int rd_idx = 0;
   int read_value;
   
#ifdef SHOW_PROGRESS
   /* start a progress bar */
   printf("Progress ");
#endif
   
   /* write n entries */
   for(i=0;i<n;i++){
      /* wait till queue is not empty (i.e. till counter !=0)  */
      while(counter==0);
     
      /* read a value from the queue */
      read_value = arr[rd_idx];
      rd_idx = (rd_idx+1)%BUF_SIZE;

      /* decrement the counter */
      sem_wait(&mutex);
      counter--;
      sem_post(&mutex);

#ifdef SHOW_PROGRESS
      /* do a progress bar */
      if(i%10000 == 0){
         printf("-");
         fflush(stdout);
      }
#endif 
     
      /* Test correctness of read value */
      if(read_value!=i){
         printf("Error: read value (%d) is out of sequence. It should be %d\n", read_value, i);
      }
   }
   
   /* Get the CPU time elapsed */
   struct timespec thread_time;
   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &thread_time);
   printf("\nThe CPU time consumed by the reader thread is %f\n", thread_time.tv_sec+thread_time.tv_nsec*1e-9);


}

int main(int argc, char *argv[])
{
   pthread_t tid;
	   
   /* Obtain the sequence size */
	if (argc == 1) {
		fprintf(stderr,"Usage: ./lab3 <starting value>\n");		
		return -1;
	}
	n = atoi(argv[1]);


	/* create the shared memory segment */
	arr = malloc(BUF_SIZE*sizeof(int));
	
	/* initialize semaphores */
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUF_SIZE);
	sem_init(&mutex, 0, 1); 

	/* create the secondary thread */
#ifdef USE_SEMS
	int err = pthread_create(&tid, NULL, reader, NULL);
#else
	int err = pthread_create(&tid, NULL, reader2, NULL);
#endif
	if(err!=0){
	   printf("Error: couldn't create the thread\n");
	   return -1;
   }
	
	/* call the writer function */
#ifdef USE_SEMS
	writer();
#else
   writer2();
#endif
	
	/* wait for child to exit */
	pthread_join(tid, NULL);
	free(arr);
	
	/* destroy the semaphores */
	sem_destroy(&full);
	sem_destroy(&empty);
	sem_destroy(&mutex);

	return 0;
}



