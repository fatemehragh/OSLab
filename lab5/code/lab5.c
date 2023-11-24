#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

// Function to print the histogram
void printHistogram(int *hist) {
    int i, j;
    for (i = 0; i < 25; i++) {
        for (j = 0; j < hist[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    // Array 'hist' with 25 elements
    int hist_serialized[25] = {0};

    // Shared memory for concurrent version
    int shmid = shmget(IPC_PRIVATE, sizeof(int) * 25, IPC_CREAT | 0666);
    int *hist_concurrent = (int *)shmat(shmid, NULL, 0);

    int counter, random_num, i, j;
    int num_samples[] = {500, 5000, 50000};

    // Serialized version
    printf("Serialized Version:\n");
    for (j = 0; j < 3; j++) {
        int num_samples_current = num_samples[j];

        clock_t start_time = clock();

        // Solve the problem for the current number of samples
        for (i = 0; i < num_samples_current; i++) {
            counter = 0;

            for (int k = 0; k < 12; k++) {
                random_num = rand() % 101;

                if (random_num >= 49) {
                    counter++;
                }
            }

            hist_serialized[counter + 12]++;
        }

        clock_t end_time = clock();

        // Calculate the execution time in seconds
        double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        printf("Number of samples: %d\tExecution time: %f seconds\n", num_samples_current, execution_time);
    }

    // Concurrent version
    printf("\nConcurrent Version (Using Fork & IPC):\n");
    for (j = 0; j < 3; j++) {
        int num_samples_current = num_samples[j];

        clock_t start_time = clock();

        pid_t pid = fork();

        if (pid == 0) {  // Child process
            for (i = 0; i < num_samples_current; i++) {
                counter = 0;

                for (int k = 0; k < 12; k++) {
                    random_num = rand() % 101;

                    if (random_num >= 49) {
                        counter++;
                    }
                }

                // Increase the corresponding value in 'hist'
                hist_concurrent[counter + 12]++;
            }

            // Detach shared memory
            shmdt(hist_concurrent);
            exit(0);
        } else if (pid > 0) {  // Parent process
            wait(NULL);
        } else {  
            fprintf(stderr, "Fork failed.\n");
            return 1;
        }
        
        clock_t end_time = clock();

        // Calculate the execution time in seconds
        double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        printf("Number of samples: %d\tExecution time: %f seconds\n", num_samples_current, execution_time);
    }

    // Print histograms
    printf("\nHistograms:\n");
    printf("Serialized:\n");
    printHistogram(hist_serialized);
    printf("\nConcurrent (Using Fork & IPC):\n");
    printHistogram(hist_concurrent);

    // Clean up shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
