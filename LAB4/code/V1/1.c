#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<string.h>
int main()
{
    key_t key = ftok("File",65);
  	char str[1000];
	void *sharedM;  
	int sharedM_id;  
	sharedM_id=shmget(key, 1024, 0666|IPC_CREAT);   
	sharedM=shmat(sharedM_id,NULL,0);   
	printf("Address: %p\n",sharedM);   
	printf("Write the data you want to save in the address:\n");  
	read(0,str,1000);  
	strcpy(sharedM,str); 
	return 0;
}  
