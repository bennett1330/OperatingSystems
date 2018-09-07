#include <stdio.h>
#include <pthread.h>
#include <time.h>

int counter;
long funtime;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *runner (void *param) {
	struct timespec start, end;
	long long conFactor = 1000000;
	long long timeDiff;
    int n = atoi(param);

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
    int i;
    pthread_mutex_lock(&lock);
    // critical section
    for (i = 0; i < n; i++)
        counter--;
    // end critical section
    pthread_mutex_unlock(&lock);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);

	timeDiff = (end.tv_nsec - start.tv_nsec)/conFactor;
	printf("timeDiff = %d ms\n\r", timeDiff);
	pthread_exit(0);
}

int main (int argc, char *argv[]) {
	counter = 0;
	pthread_t tid;
	pthread_attr_t attr;
	if (argc != 2)
		fprintf(stderr, "usage: Lab7 <integer>\n\r");
    int n = atoi(argv[1]);
    if ((n == 0) && (argv[1] != "0"))
		fprintf(stderr, "usage: not a valid input.\n\r");
	pthread_attr_init(&attr);
	int rc = pthread_create(&tid, &attr, runner, argv[1]);
	if (rc)
		fprintf(stderr, "pthread_create error code: %d.\n\r", rc);

	pthread_mutex_lock(&lock);
	// critical section
	int i;
	for (i = 0; i < n; i++)
		counter++;
	// end critical section
	pthread_mutex_unlock(&lock);

	pthread_join(tid, NULL);
}
