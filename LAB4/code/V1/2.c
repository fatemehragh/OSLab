#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>  
int main()  
{  
	key_t key = ftok("File",65);  
	void *sharedM;  
	char str[1000];  
	int sharedM_id;  
	sharedM_id=shmget(key, 1024, 0444);
	sharedM=shmat(sharedM_id,NULL,0);  
	printf("Address: %p\n",sharedM);  
	printf("The value stored in this address is: %s\n",(char *)sharedM);
	return 0;
}
