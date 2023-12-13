#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_COUNT 15

sem_t reader_writer;
sem_t mutex;

int numberReader = 0;
int *cnt = 0;


void *writer(void *wno)
{   
    *cnt = 0;
    int shmid = shmget(0x125, 10*sizeof(int), 0666 | IPC_CREAT);
    cnt = (int *)shmat(shmid, NULL, 0);
    sem_wait(&reader_writer);
    for (int i = 1; i <= MAX_COUNT; i++)
        {
            
            *cnt=*cnt+1;
            printf("Writer %d write cnt --> %d\n",(*((int *)wno)),*cnt);
        }
    sem_post(&reader_writer);

}

void *reader(void *rno)
{   
    
    int shmid = shmget(0x125, 10*sizeof(int), 0444 | IPC_CREAT);
    cnt = (int *)shmat(shmid, NULL, 0);
    sem_wait(&mutex);
    numberReader++;
    if(numberReader == 1) {
        sem_wait(&reader_writer); 
    }
    sem_post(&mutex);
    printf("Reader %d: read cnt --> %d\n",*((int *)rno),*cnt);

    sem_wait(&mutex);
    numberReader--;
    if(numberReader == 0) {
        sem_post(&reader_writer); 
    }
    sem_post(&mutex);
}

int main()
{   
    int shmid = shmget(0x125, 10 * sizeof(int), 0666 | IPC_CREAT);
    cnt = (int *)shmat(shmid, NULL, 0);
    pthread_t read[10],write[5];
    sem_init(&mutex,0,1);
    sem_init(&reader_writer,0,1);

    int a[10] = {1,2,3,4,5};

    for(int i = 0; i < 5; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 1; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 1; i++) {
        pthread_join(write[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&reader_writer);

    return 0;
    
}
