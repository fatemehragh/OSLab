#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXDATALEN 500 


int sock;
struct sockaddr_in serv_addr;
char buff[MAXDATALEN];
char username[10];

void *writeChat(int sock)
{
    while (1)
    {
        fgets(buff, MAXDATALEN,stdin);

        send(sock, buff, strlen(buff), 0);

        if (strcmp(buff, "quit") == 0)
            exit(0);

        bzero(buff, MAXDATALEN);
    }
}

void *readChat(int sock)
{
    while (1)
    {
        int n = recv(sock, buff, MAXDATALEN, 0);
        if (n == 0)
        {
            printf("bye!\n");
            exit(0);
        }

        printf("%s ", buff);
        bzero(buff, MAXDATALEN);
    }
}
int main(int argc, char *argv[])
{

    pthread_t readThread, writeThead; 

    if (argc != 4)
    {
        printf("invalid args!\n");
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error! \n"); 
        return -1;
    }

    
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

  
    strcpy(username, argv[3]);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("connection failed!\n");
        return -1;
    }

    printf("you joined.\n");

    send(sock, username, strlen(username), 0);


    pthread_create(&writeThead, NULL, (void *)writeChat, (void *) (intptr_t) sock);
    pthread_create(&readThread, NULL, (void *)readChat, (void *) (intptr_t) sock); 
    pthread_join(writeThead, NULL);
    pthread_join(readThread, NULL);

    return 0;

}




