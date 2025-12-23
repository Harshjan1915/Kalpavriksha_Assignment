#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/types.h>

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(){
    key_t key;
    int shmID;
    int* shm;
    int n;
    scanf("%d",&n);

    key = ftok("data",77);

    shmID = shmget(key , sizeof(int)*n , 0666 | IPC_CREAT);

    shm = (int *)shmat(shmID , NULL , 0);

    for(int i = 0 ; i < n ; i++){
        scanf("%d",&shm[i]);
    }

    printf("before sorting:\n");
    for(int i = 0 ; i < n ; i++){
        printf("%d\t",shm[i]);
    }

    printf("\n");
    int id = fork();

    if( id == 0 ){

        qsort(shm , n ,sizeof(int) ,cmp);
        shmdt(shm);

    }else{

        wait(NULL);

        printf("After sorting:\n");
        for(int i = 0 ; i < n ; i++){
            printf("%d\t",shm[i]);
        }
        printf("\n");
        shmdt(shm);
    }
    return 0;
}
