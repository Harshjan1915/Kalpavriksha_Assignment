#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t serverMutex;

void* clientRequestHandler(void* args) {
    int clientSocket = *((int *)args);
    free(args);

    int choice, amount, balance;
    FILE* fp;

    while (1) {
        if (recv(clientSocket, &choice, sizeof(int), 0) <= 0)
            break;

        pthread_mutex_lock(&serverMutex);

        fp = fopen("./resource/accountDB.txt", "r+");
        if (!fp) {
            pthread_mutex_unlock(&serverMutex);
            break;
        }

        fscanf(fp, "%d", &balance);

        if (choice == 1) { 
            recv(clientSocket, &amount, sizeof(int), 0);

            if (amount > balance) {
                int fail = -1;
                send(clientSocket, &fail, sizeof(int), 0);
            } else {
                balance -= amount;
                rewind(fp);
                fprintf(fp, "%d", balance);
                fflush(fp);
                send(clientSocket, &balance, sizeof(int), 0);
            }
        }
        else if (choice == 2) { 
            recv(clientSocket, &amount, sizeof(int), 0);
            balance += amount;
            rewind(fp);
            fprintf(fp, "%d", balance);
            fflush(fp);
            send(clientSocket, &balance, sizeof(int), 0);
        }
        else if (choice == 3) { 
            send(clientSocket, &balance, sizeof(int), 0);
        }

        fclose(fp);
        pthread_mutex_unlock(&serverMutex);
    }

    close(clientSocket);
    return NULL;
}

int main(){

    pthread_mutex_init(&serverMutex , NULL);
    int serverSocket = socket( AF_INET , SOCK_STREAM , 0 );

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8001);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket , (struct sockaddr*)&serverAddress , sizeof(serverAddress));

    listen( serverSocket , 5 );
    int clientSocket;
    while(1){
        clientSocket = accept(serverSocket , NULL ,NULL);
        int* CSocket = calloc(1, sizeof(int));
        *CSocket=clientSocket;

        pthread_t thread;
        pthread_create(&thread , NULL , &clientRequestHandler , CSocket);
        pthread_detach(thread);
    }

    close(serverSocket);
    pthread_mutex_destroy(&serverMutex);
    return 0;
}