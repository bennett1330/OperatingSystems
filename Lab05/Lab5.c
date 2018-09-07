#include <stdio.h>
#include <pthread.h>

int counter;

void *runner (void *param) {
        int n = atoi(param);
        int i = 0;
        for (i; i < n; i++)
                counter--;
        pthread_exit(0);
}


int main (int argc, char *argv[]) {
	counter = 0;
	pthread_t tid;
	pthread_attr_t attr;
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
	if (rc)
		fprintf(stderr, "pthread_create error code: %d.\n\r", rc);
	int i;
	for (i = 0; i < n; i++)
		counter++;
	pthread_join(tid, NULL);
        printf("n = %s, counter = %d \n\r", argv[1], counter);
}
