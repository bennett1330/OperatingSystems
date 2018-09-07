#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#define N 4

int in;
int out;
int count;
char queue_str[N];

void *runner(void *param) {
	int *length = (int*) param;
    do {
        while (in == out) {
        }
        printf("%c", queue_str[out]);
        out = (out + 1) % N;
    } while (count - 1 <= *length);
    pthread_exit(0);
}

int main() {
	in = 0;
	out = 0;
	count = 0;
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
		while (((in + 1) % N) == out) {
		}
		queue_str[in] = input_str[count];
		in = (in + 1) % N;
		count++;
	} while (count - 1 <= length);
	
	rc = pthread_join(tid, NULL);
	if (rc) {
		fprintf(stderr, "Error joining thread. Code: %d.\n\r", rc);
		return 2;
	}
	printf("\n\r");
}
