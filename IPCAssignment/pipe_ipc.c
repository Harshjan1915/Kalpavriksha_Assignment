#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(){
    int n;
    scanf("%d",&n);
    int arr[n];

    int fd[2];
    if(pipe(fd) != 0){
        printf("creating pipe fails\n");
        return 1;
    }
    for(int i = 0 ; i < n ; i++){
        scanf("%d",&arr[i]);
    }

    printf("before sorting:\n");
    for(int i = 0 ; i < n ; i++){
        printf("%d\t",arr[i]);
    }

    printf("\n");

    int id = fork();

    if( id == 0 ){
        read(fd[0] , arr , sizeof(int) *n);
        close(fd[0]);
        qsort(arr , n ,sizeof(int) ,cmp);
        write(fd[1] , arr , sizeof(int) * n);
        close(fd[1]);
    }else{

        write(fd[1] , arr, sizeof(int)*n);
        close(fd[1]);
        wait(NULL);

        read(fd[0] , arr , sizeof(int)*n);
        close(fd[0]);

        printf("After sorting:\n");
        for(int i = 0 ; i < n ; i++){
            printf("%d\t",arr[i]);
        }
        printf("\n");
    }
    return 0;
}