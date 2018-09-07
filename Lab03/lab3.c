#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define BUF_SZ 10

int main(int argc, char *argv[])
{
	typedef struct 
	{

	} item;
	item buffer[BUF_SZ];
	int in = 0;
	int out = 0;
	pid_t pid;
	const int SIZE = 4096;
	const char *name = "memory";

	int shm_fd;
	void *ptr;
	int *memptr;
	int number;
	pid = fork();
	sscanf(argv[1],"%d",&number);
	//int number = (*argv[1] - '0');
	
	if (pid == 0)
	{
		shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, SIZE);
		ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

		int first=1, second=1, sum, i;
		/*printf("how many numbers in the sequence?"); 
		scanf("%d", &number);		//getting the number of entries in the sequence*/
		//sprintf(ptr, "%d ", number);
		//ptr += 16; 
		sprintf(ptr, "%d ", first);		//printing out the first value of the sequence
		ptr +=16;
		sprintf(ptr, "%d ", second);		//printing out the second value of the sequence
		ptr += 16;
		for(i=2; i<number; ++i)
		{
			sum=first+second;	//calculating the next value of the sequence
			sprintf(ptr,"%d ",sum);	//printing out the next value in the sequence
			first=second;		//incrementing the "first" value
			second=sum;		//incrementing the "second" value
			ptr += 16;
			usleep(1000 * (rand()%1000) );
			while(((in + 1) % BUF_SZ) == out);
			{
				buffer[in] = sum;
				in = (in + 1) % BUF_SZ;
			}
		}
		//printf("\n");			//adds a new line to make it look cleaner in terminal
	
	}
	else if (pid < 0)
	{
		printf("Fork failed");
		return 1;
	}
	else
	{
		wait(NULL);
		shm_fd = shm_open(name, O_RDONLY, 0666);
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
		memptr = (int *)ptr;
		int i;
		//int number = &(int *)ptr;
		//printf("%d \n", memptr);
		//ptr+=16;
		//printf("%d \n", number);
		//printf("%d \n", argc);
		next_consumed = ((char *)ptr)
		for(i=1; i<(number+1); ++i)
		{
			printf("%s \n",next_consumed);
			ptr+=16;
			//printf("%s \n",(char *)ptr);
			while (in==out);
			{
				next_consumed = buffer[out];
				out = (out + 1) % BUF_SZ;
			}
		}
		shm_unlink(name);
		return 0;
	}
}

