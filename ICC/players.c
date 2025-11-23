#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "players.h"
#include "teams.h"

int isCorrectPID(int pid){
    if(pid < 1 || pid > 1000){
        printf("Player ID must be in range 1 to 1000.\n");
        return 0;
    }
    for(int i=0;i< teamCount;i++){
        struct PlayerNode *bat=teamList[i].batsmen;
        while(bat != NULL){
            if(bat->player->playerId == pid){
                printf("Player ID %d already exists. Cannot add duplicate Player ID.\n", pid);
                return 0;
            }
            bat = bat->next;
        }
        struct PlayerNode *bowl=teamList[i].bowlers;
        while(bowl != NULL){
            if(bowl->player->playerId == pid){
                printf("Player ID %d already exists. Cannot add duplicate Player ID.\n", pid);
                return 0;
            }
            bowl = bowl->next;
        }
        struct PlayerNode *allRounder=teamList[i].allRounders;
        while(allRounder != NULL){
            if(allRounder->player->playerId == pid){
                printf("Player ID %d already exists. Cannot add duplicate Player ID.\n", pid);
                return 0;
            }
            allRounder = allRounder->next;
        }
    }
    return 1;
}

float calculatePerformanceIndex(enum Role role, float battingAverage, float strikeRate, int wickets, float economyRate){
    float performanceIndex = 0.0;
    if(role == Batsman){
        performanceIndex = (battingAverage * strikeRate )/100;  
    }else if(role == Bowler){
        performanceIndex = (wickets * 2.0) - (100-economyRate);
    }else if(role == AllRounder){
        performanceIndex = ((battingAverage*strikeRate)/100) + (wickets * 2.0);
    }
    return performanceIndex;
}

void insertSorted(struct PlayerNode **head, struct PlayerNode *newNode){
    if(*head==NULL || (*head)->player->performanceIndex < newNode->player->performanceIndex){
        newNode->next = *head;
        *head = newNode;
        return;
    }

    struct PlayerNode *temp=*head;
    while(temp->next !=NULL && temp->next->player->performanceIndex >= newNode->player->performanceIndex){
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}

void displayPlayerByRole(struct PlayerNode *temp){
    while(temp != NULL){
        if(temp->player->role == Batsman){
            printf("%d\t%-25sBatsman\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->player->playerId, temp->player->playerName, temp->player->totalRuns, temp->player->battingAverage, temp->player->strikeRate, temp->player->wickets, temp->player->economyRate, temp->player->performanceIndex);
        }
        else if(temp->player->role == Bowler){
            printf("%d\t%-25sBowler \t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->player->playerId, temp->player->playerName, temp->player->totalRuns, temp->player->battingAverage, temp->player->strikeRate, temp->player->wickets, temp->player->economyRate, temp->player->performanceIndex);
        }
        else{
            printf("%d\t%-25sAll-Rounder\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->player->playerId, temp->player->playerName, temp->player->totalRuns, temp->player->battingAverage, temp->player->strikeRate, temp->player->wickets, temp->player->economyRate, temp->player->performanceIndex);
        }
        temp = temp->next;
    }

}

void freeMemory(){
    for(int i=0;i< teamCount;i++){
        struct PlayerNode *temp;
        temp = teamList[i].batsmen;
        while(temp != NULL){
            struct PlayerNode *toFree = temp;
            temp = temp->next;
            free(toFree->player);
            free(toFree);
        }
        temp = teamList[i].bowlers;
        while(temp != NULL){
            struct PlayerNode *toFree = temp;
            temp = temp->next;
            free(toFree->player);
            free(toFree);
        }
        temp = teamList[i].allRounders;
        while(temp != NULL){
            struct PlayerNode *toFree = temp;
            temp = temp->next;
            free(toFree->player);
            free(toFree);
        }
    }
}