#include <stdio.h>
#include <pthread.h>
#include <time.h>

int counter;
long funtime;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *runner (void *param) {
        int n = atoi(param);
        int i = 0;
        struct timespec timerfunstart, timerfunend;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &timerfunstart);
        for (i; i < n; i++) {
        	pthread_mutex_lock(&lock);
            counter--;
            pthread_mutex_unlock(&lock);
        }
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &timerfunend);
        funtime = (timerfunend.tv_nsec - timerfunstart.tv_nsec)/1000000;
        pthread_exit(0);
}


int main (int argc, char *argv[]) {
	counter = 0;
	pthread_t tid;
	pthread_attr_t attr;
	struct timespec timermainstart, timermainend;

	//pthread_mutex_t lock;
	//pthread_mutex_init(lock, NULL);

	if (argc != 2) {
		fprintf(stderr, "usage: Lab5 <integer>\n\r");
		return -1;
	}
    int n = atoi(argv[1]);
    if ((n == 0) && (argv[1] != "0")) {
		fprintf(stderr, "usage: not a valid input.\n\r");
		return -1;
    }
	pthread_attr_init(&attr);
	int rc = pthread_create(&tid, &attr, runner, argv[1]);
	if (rc){
		fprintf(stderr, "pthread_create error code: %d.\n\r", rc);
	}
	int i;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &timermainstart);
	for (i = 0; i < n; i++) {
		pthread_mutex_lock(&lock);
		counter++;
		pthread_mutex_unlock(&lock);
	}
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &timermainend);
	pthread_join(tid, NULL);
//	pthread_mutex_destroy(lock);

	long timermain = (timermainend.tv_nsec - timermainstart.tv_nsec)/1000000;
    printf("n = %s, counter = %d \n\r", argv[1], counter);
    printf("timermain = %d, timerfun = %d\n", timermain, funtime);
}
