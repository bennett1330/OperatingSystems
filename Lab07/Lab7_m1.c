#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

int counter;
int turn;
bool req[2];

void *runner (void *param) {
	struct timespec start, end;
	long long conFactor = 1000000;
	long long timeDiff;
    int n = atoi(param);

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
	do {
		req[1] = true;
		turn = 0;
		while (req[0] && turn == 0) {
		}
		// critical section
	    int i;
        for (i = 0; i < n; i++)
            counter--;
		// end critical section
		req[1] = false;
	} while ((counter > 0));
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

	do {
		req[0] = true;
		turn = 1;
		while (req[1] && turn == 1){
		}
		// critical section
		int i = 0;
        for (i; i < n; i++)
            counter++;
		// end critical section
		req[0] = false;
	} while (counter+1 < n);

	rc = pthread_join(tid, NULL);
	return 0;
}
