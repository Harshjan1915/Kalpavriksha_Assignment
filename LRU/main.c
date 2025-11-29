#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "lru.h"

int main(){
    char command[20];
        
    struct HashMap* hashMap[hashSize];
    for(int i=0;i<hashSize;i++){
        hashMap[i]=NULL;
    }
    Cache* cache=NULL;
    while(1){
        scanf("%s", command);
        if(strcmp(command, "createCache")==0){
            int capacity;
            scanf("%d", &capacity);
            cache=createCache(capacity);
        }
        else if(strcmp(command, "put")==0){
            int key;
            char value[20];
            scanf("%d", &key);
            scanf("%s", value);
            put(key,value,hashMap,cache);            
        }
        else if(strcmp(command, "get")==0){
            int key;
            scanf("%d", &key);
            char* value=get(key,hashMap,cache);
            if(value==NULL){
                printf("NULL\n");
            }
            else{
                printf("%s\n", value);
            }
        }
        else if(strcmp(command, "exit")==0){
            
            for(int i=0;i<hashSize;i++){
                struct HashMap* temp=hashMap[i];
                while(temp!=NULL){
                    struct HashMap* temp2=temp;
                    temp=temp->next;
                    free(temp2);
                }
            }
            free(cache);
            break;
        }
        else{
            printf("Invalid command\n");
        }
    }

    return 0;
}