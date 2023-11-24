#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define R_END 0
#define W_END 1

char message[100] = "asadfEFFGFFsddxcsWQW";

int main(){
    int p1[2];
    int p2[2];

    pid_t pid;

    if(pipe(p1) == -1){   
        perror("pipe1 error");
        exit(EXIT_FAILURE);
    }

    if(pipe(p2) == -1){   
        perror("pipe2 error");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == -1){   
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    
    
    if (pid != 0) {  

        close(p1[R_END]);

       
        close(p2[W_END]);

        printf("%s", message);

        write(p1[W_END], message, sizeof(message));       
        close(p1[W_END]);   

	wait(NULL);

        read(p2[R_END], message, sizeof(message));
        close(p2[R_END]);

        printf(" -----> %s\n", message);

    }
    
    
    else {
        close(p1[W_END]);
        close(p2[R_END]);


        char response[strlen(message)];

        read(p1[R_END], response, sizeof(response));
        close(p1[0]);

        for(int i=0; i < strlen(response) ; i++){
            if (islower(response[i])) {
               response[i]=toupper(response[i]);
               }
            else{
              response[i] = tolower(response[i]);
              }  
        }

        write(p2[W_END], response, sizeof(response));
        close(p2[W_END]);  
        }
    return 0;
}
