#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>


#define MAXDATALEN 500 


typedef struct
{
    int id;
    char username[10];
} User;



void *cHandle(void *arg);
void userAdd(int id, char *username, User *group, int *tail);
void userRemove(int id, User *group, int *tail);
int searchMember(int id, User *group, int tail);

char username[10];
User users[10] = {0};
int group_tail[10] = {0};
int userIndex = 0; 
char buff[MAXDATALEN];
User groups[10][10] = {0};



int main(int argc, char *argv[])
{
    struct sockaddr_in serverAddress; 
    struct sockaddr_in client_addr;
    int ServerFD, ClientFD; 
    int portNumber; 
    pthread_t thr;
    int clientSize;

    if (argc != 2)
    {
        printf("invalid args\n");
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
    serverAddress.sin_port = htons(atoi(argv[1]));

    ServerFD = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(ServerFD, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr)) < 0)
    {
       perror("bind failed"); 
       exit(EXIT_FAILURE);
    }

    if(listen(ServerFD, 100) < 0){
        perror("listenning failed"); 
        exit(EXIT_FAILURE);
    }
    printf("listening for clients...\n");


    while (1)
    {
        clientSize = sizeof(struct sockaddr_in); 
        ClientFD = accept(ServerFD, (struct sockaddr *)&client_addr, &clientSize);

        bzero(username, 10);
        int t = recv(ClientFD, username, sizeof(username), 0);
        username[strlen(username)+1] = ':';
        printf("-- %s joined --\n", username);
        userAdd(ClientFD, username, users, &userIndex);

        User args;
        args.id = ClientFD;
        strcpy(args.username, username);

        pthread_create(&thr, NULL, cHandle, (void *)&args); 
        pthread_detach(thr);
    }
    close(ServerFD);
}

void *cHandle(void *arguments)
{
    User *args = arguments;
    char clientUsername[10]; 
    strcpy(clientUsername, args->username);
    int clientID = args->id;

    char *massage = (char *)malloc(MAXDATALEN);
    int massageLenght;
    char buff[MAXDATALEN];

    while (1)
    {
        bzero(buff, MAXDATALEN); 
        massageLenght = recv(clientID, buff, MAXDATALEN, 0);

       
        if (strncmp(buff, "join", 4) == 0)
        {
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strcpy(group_id_str, buff + 5);
            int group_id = atoi(group_id_str);
            printf("-- %s joined group %d --\n", clientUsername, group_id);
            userAdd(clientID, clientUsername, groups[group_id], &group_tail[group_id]);
        }
        
        else if (strncmp(buff, "send", 4) == 0)
        {
            int group_id = atoi(buff + 5);
            if (searchMember(clientID, groups[group_id], group_tail[group_id]) != -1)
            {
            	char *msg = malloc(sizeof(MAXDATALEN));
            	strcpy(msg, clientUsername);
            	msg[strlen(clientUsername)]=':';
            	strcat(msg,buff+6);
            	printf("%s\n",msg);
            	for (int i = 0; i < group_tail[group_id]; i++)
            	 {
                	if (groups[group_id][i].id != clientID)
                    	send(groups[group_id][i].id, msg, massageLenght, 0);
            	 }

            		bzero(massage, MAXDATALEN);
            }

        }
        else if (strncmp(buff, "leave", 5) == 0)
        {
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strcpy(group_id_str, buff + 6);
            int group_id = atoi(group_id_str);
            printf("-- %sleft group %d --\n",clientUsername , group_id);
            userRemove(clientID, groups[group_id], &group_tail[group_id]);
        }


         else if (strncmp(buff, "quit", 4) == 0)
        {

            printf("-- %s left chat -- \n", clientUsername);
            userRemove(clientID, users, &userIndex);

            for (int i = 0; i < 10; i++)
            {
                userRemove(clientID, groups[i], &group_tail[i]);
            }

            close(clientID);
            break;
        }
        
    }
    return 0;
}



void userAdd(int id, char *username, User *group, int *tail)
{
    if (searchMember(id, group, *tail) != -1)
    {
        return;
    }
    User *temp;
    temp = malloc(sizeof(User));
    temp->id = id;
    strcpy(temp->username, username);
    group[(*tail)++] = *temp;
    printf("%s\n","added successfully.");
}

int searchMember(int id, User *group, int tail)
{
    for (int i = 0; i < tail; i++)
    {
        if (group[i].id == id)
            printf("%s\n","founded.");
            return i;
    }
    printf("%s\n","not found!");
    return -1;
}

void userRemove(int id, User *group, int *tail)
{
    int index = searchMember(id, group, *tail);
    if (index == -1)
    {
        return;
    }

    for (int i = index; i < *tail - 1; i++)
    {
        group[i] = group[i + 1];
    }
    (*tail)--;
}



