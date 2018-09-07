#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#define N 4

int in;
int out;
int count;
int count_queue;
int turn;
bool req[2];
char queue_str[N];

void *runner(void *param) {
	int *length = (int*) param;
	do {
		req[1] = true;
		turn = 0;
		while (req[0] && turn == 0) {
		}
		if (count_queue > 0) {
			printf("%c", queue_str[out]);
			out = (out + 1) % N;
			count_queue--;
		}
		req[1] = false;
	} while ((count <= *length) || (count_queue > 0));
	pthread_exit(0);
}

int main() {
	in = 0;
	out = 0;
	count = 0;
	count_queue = 0;
	char input_str[] = "Hello world, this is COM3499!";
	int length = strlen(input_str);
	pthread_t tid;
	pthread_attr_t attr;

	int rc = pthread_create(&tid, NULL, runner, &length);
	if (rc) {
		fprintf(stderr, "Error creating thread. Code: %d.\n\r", rc);
		return 1;
	}

	do {
		req[0] = true;
		turn = 1;
		while (req[1] && turn == 1){
		}
		if (count_queue < N) {
			queue_str[in] = input_str[count];
			in = (in + 1) % N;
			count_queue++;
			count++;
		}
		req[0] = false;
	} while ((count <= length));
	rc = pthread_join(tid, NULL);
	if (rc) {
		fprintf(stderr, "Error joining thread. Code: %d.\n\r", rc);
		return 2;
	}
	printf("\n\r");
}
