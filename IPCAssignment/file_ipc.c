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
        FILE* fp = fopen("data.txt" , "r");
        fread(arr , sizeof(int) , n , fp);
        fclose(fp);

        qsort(arr , n ,sizeof(int) ,cmp);

        fp=fopen("data.txt","w");
        if(fp==NULL){
            printf("error in opening file.\n");
            exit(1);
        }
        fwrite(arr , sizeof(int) ,n ,fp);
        fclose(fp);
        fclose(fp);

    }else{

        FILE* fp=fopen("data.txt","w");
        if(fp==NULL){
            printf("error in opening file.\n");
            exit(1);
        }
        fwrite(arr , sizeof(int) ,n ,fp);
        fclose(fp);

        wait(NULL);

        fp=fopen("data.txt","r");
        fread(arr , sizeof(int) , n , fp);

        fclose(fp);
        printf("after sorting:\n");
        for(int i = 0 ; i < n ; i++){
            printf("%d\t",arr[i]);
        }
    }
    return 0;
}