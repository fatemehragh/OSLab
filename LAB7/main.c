#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#define NUMBER_OF_RESOURCES 6

/* this maybe any values >= 0 */
#define NUMBER_OF_CUSTOMERS 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need [NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int request_resources(int customer_num, int request[]);
bool not_greater_than_available(int request[]);
void release_resources(int customer_num, int request[]);
bool is_safe(int customer_num);
bool not_greater_than(int need[], int work[]);
bool finished(bool finish[]);


void *run(void *customer_num) 
{
    int num = *((int*) (customer_num));
    printf("Thread %d started\n", num);

    pthread_mutex_lock(&mutex);
    // initialize the arrays for customer number *num
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++) 
    {
        maximum[num][i] = rand() % available[i];
        need[num][i] = maximum[num][i];
        allocation[num][i] = 0;
    }
    pthread_mutex_unlock(&mutex);

    for(int i = 0; i < 10; i++) 
    {
        int request[NUMBER_OF_RESOURCES];
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++) 
        {
            // generate a random request for each resource ensuring that it is less than or equal to the need
            request[j] = rand() % (need[num][j] + 1);
        }
        pthread_mutex_lock(&mutex);
        int request_result = request_resources(num, request);
        pthread_mutex_unlock(&mutex);
        if(request_result == 0) 
        {
            printf("Customer %d request granted!\n", num);
            sleep(1);
            pthread_mutex_lock(&mutex);
            release_resources(num, request);
            pthread_mutex_unlock(&mutex);
            printf("Customer %d released resources!\n", num);
        } 
        else 
        {
            printf("Customer %d request denied!\n", num);
            sleep(1);
        }
    }

    return NULL;
}

int request_resources(int customer_num, int request[]) 
{
    if(not_greater_than_available(request)) 
    {
        //pretend to have allocated the requested resources
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            available[i] -= request[i];
            allocation[customer_num][i] += request[i];
            need[customer_num][i] -= request[i];
        }
        // check if the system is safe
        if(is_safe(customer_num)) 
        {
            return 0;
        } 
        else 
        {
            // if not safe, undo the allocation
            release_resources(customer_num, request);
            return -1;
        }
    }
    return -1;
}

bool not_greater_than_available(int request[]) 
{
    bool output = true;
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++) 
    {
        output = output && (request[i] <= available[i]);
    }
    return output;
}

void release_resources(int customer_num, int request[]) 
{
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++) 
    {
        need[customer_num][i] += request[i];
        allocation[customer_num][i] -= request[i];
        available[i] += request[i];
    }
}

bool is_safe(int customer_num) 
{
    // create a copy of the available array
    int work[NUMBER_OF_RESOURCES];
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++) 
    {
        work[i] = available[i];
    }

    // initialize the finish array
    bool finish[NUMBER_OF_CUSTOMERS];
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++) 
    {
        finish[i] = false;
    }

    // loop through the need array until all customers are finished
    // it finds the first customer that is not finished and has a need that is less than or equal to the work
    // if it finds one, it adds the allocation of that customer to the work and marks the customer as finished
    // if it doesn't find one, it returns false
    for(int i = 0; i < NUMBER_OF_RESOURCES * NUMBER_OF_RESOURCES; i++) 
    {
        int index = i % NUMBER_OF_RESOURCES;
        if(!finish[index] && not_greater_than(need[customer_num], work)) 
        {
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++) 
            {
                work[j] += allocation[customer_num][j];
            }
            finish[index] = true;
        }
    }

    return finished(finish);
}

bool not_greater_than(int need[], int work[]) 
{
    bool output = true;
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++) 
    {
        output = output && (need[i] <= work[i]);
    }
    return output;
}

bool finished(bool finish[]) 
{
    bool output = true;
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++) 
    {
        output = output && finish[i];
    }
    return output;
}

int main(int argc, char** argv) 
{
    srand(time(0));
    if(argc < 7) 
    {
        printf("not enough arguments\n");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < 6; i++) 
    {
        available[i] = strtol(argv[i + 1], NULL, 10);
    }
    for(int i = 0; i < 6; i++) 
    {
        printf("av[%d]: %d\n", i, available[i]);
    }

    pthread_t customerThreads[4];
    int i = 0, j = 1, k = 2, l = 3;
    pthread_create(&customerThreads[i], NULL, run, (void*) &i);
    pthread_create(&customerThreads[j], NULL, run, (void*) &j);
    pthread_create(&customerThreads[k], NULL, run, (void*) &k);
    pthread_create(&customerThreads[l], NULL, run, (void*) &l);
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_join(customerThreads[i], NULL);
    }
}