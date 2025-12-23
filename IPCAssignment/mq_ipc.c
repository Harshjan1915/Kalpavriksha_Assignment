#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/types.h>

#define MAX 100

struct message {
    long mtype;       
    int n;             
    int arr[MAX];     
};

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(){
    key_t key;
    int msgID;
    struct message msg;
    scanf("%d",&msg.n);

    key = ftok("data",77);

    msgID = msgget(key , 0666 | IPC_CREAT);

    for(int i = 0 ; i < msg.n ; i++){
        scanf("%d",&msg.arr[i]);
    }

    printf("before sorting:\n");
    for(int i = 0 ; i < msg.n ; i++){
        printf("%d\t",msg.arr[i]);
    }
    printf("\n");

    int id = fork();

    if( id == 0 ){

        msgrcv(msgID , &msg , sizeof(msg) - sizeof(long) ,1 , 0);
        qsort( msg.arr , msg.n ,sizeof(int) ,cmp);
        msg.mtype=2;
        msgsnd(msgID , &msg , sizeof(msg) - sizeof(long) , 0);
        exit(0);

    }else{

        msg.mtype=1;
        msgsnd(msgID , &msg , sizeof(msg) - sizeof(long)  , 0);
        wait(NULL);

        msgrcv(msgID , &msg , sizeof(msg) - sizeof(long) ,2 , 0);
        printf("After sorting:\n");
        for(int i = 0 ; i < msg.n ; i++){
            printf("%d\t",msg.arr[i]);
        }
        printf("\n");
        msgctl(msgID , IPC_RMID , NULL);
    }
    return 0;
}