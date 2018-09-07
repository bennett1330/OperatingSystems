#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>


int main(int argc, char *argv[])
{

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
	int fd[2];
	pipe(fd);
	
	if (pid == 0)
	{
		/*shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, SIZE);
		ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);*/
		close(fd[0]);
		int first=1, second=1, sum, i;
		char string[15];
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
			sprintf(string,"%d ",sum);	//printing out the next value in the sequence
			first=second;		//incrementing the "first" value
			second=sum;		//incrementing the "second" value
			ptr += 16;
			write(fd[1],string,16);
			//usleep(1000 * (rand()%1000) );
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
		/*shm_fd = shm_open(name, O_RDONLY, 0666);
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);*/
		//memptr = (int *)ptr;
		int i;
		char output[15];
		int stuff;
		//int number = &(int *)ptr;
		//printf("%d \n", memptr);
		//ptr+=16;
		//printf("%d \n", number);
		//printf("%d \n", argc);
		close(fd[1]);
		for(i=1; i<(number+1); ++i)
		{
			stuff = read(fd[0], output, 16);			
			printf("%s \n",output);
			ptr+=16;
			//printf("%s \n",(char *)ptr);
		}
		shm_unlink(name);
		return 0;
	}
}

