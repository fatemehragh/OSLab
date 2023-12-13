#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILSOOPH_NUM 5

pthread_mutex_t chopsticks[PHILSOOPH_NUM];


void *philosoph_handler (void* philId)
 {
  int right = *((int*)philId);
  int left = (right+1)% PHILSOOPH_NUM ;
  int rightForklock;
  int leftForklock;

  printf("Philosopher[%d] is Thinking... \n", right);
  sleep(rand() % 10);

  while(1){
  	
    rightForklock = pthread_mutex_trylock(&chopsticks[right]);
    leftForklock = pthread_mutex_trylock(&chopsticks[left]);

    if (rightForklock == 0){
    	if (leftForklock == 0){
    		printf("Philosopher[%d] is Eating by chopstick[%d] and chopstick[%d]...\n", right, right, left);
  			sleep(0.3);
  			printf("Philosopher[%d] finished eating!\n", right);
 			pthread_mutex_unlock(&chopsticks[right]);
 			pthread_mutex_unlock(&chopsticks[left]);
 			break;
    		}
    	else {
    		pthread_mutex_unlock(&chopsticks[right]);
    	    }
	}
    if (leftForklock == 0) {
		pthread_mutex_unlock(&chopsticks[left]);
	}
  }
}

void main() {
  pthread_t philosophs[PHILSOOPH_NUM];
  int ids[5];
  for (int i = 0; i < PHILSOOPH_NUM; i++) {
    ids[i] = i;
    pthread_mutex_init(&chopsticks[i], NULL);
  }

  for (int i = 0; i < PHILSOOPH_NUM; i++) {
    pthread_create(&philosophs[i], NULL, philosoph_handler, &ids[i]);
  }
  for (int i = 0; i < PHILSOOPH_NUM; i++) {
    pthread_join(philosophs[i], NULL);
  }

}

