#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "Players_data.h"

#define MAX_TEAMS 10
#define MAX_PLAYERS 50
#define MIN_PLAYERS 11

enum Role {Batsman=1,Bowler,AllRounder};

struct Players{
    int playerId;
    char playerName[50];
    char teamName[50];
    enum Role role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
};

struct PlayerNode{
    struct Players *player;
    struct PlayerNode *next;
};


struct Teams{
    int teamId;
    char teamName[50];
    int totalPlayers;
    float averageBattingStrikeRate;
    struct PlayerNode *batsmen;
    struct PlayerNode *bowlers;
    struct PlayerNode *allRounders;
};
struct Teams teamList[MAX_TEAMS];

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
void loadData(){
    for(int i=0;i< teamCount;i++){
        teamList[i].teamId = i+1;
        strcpy(teamList[i].teamName, teams[i]);
        teamList[i].totalPlayers = 0;
        teamList[i].averageBattingStrikeRate = 0.0;
        teamList[i].batsmen = NULL;
        teamList[i].bowlers = NULL; 
        teamList[i].allRounders = NULL;
    }

    for(int i=0;i< playerCount;i++){
        struct Players *newPlayer = (struct Players*)malloc(sizeof(struct Players));

        newPlayer->playerId = players[i].id;
        strcpy(newPlayer->playerName, players[i].name);
        strcpy(newPlayer->teamName, players[i].team);

        if(strcmp(players[i].role, "Batsman") == 0){
            newPlayer->role = 1;
        }else if(strcmp(players[i].role, "Bowler") == 0){
            newPlayer->role = 2;
        }else{
            newPlayer->role = 3;
        }

        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->battingAverage = players[i].battingAverage;
        newPlayer->strikeRate = players[i].strikeRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->economyRate = players[i].economyRate;

        newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer->role, newPlayer->battingAverage, newPlayer->strikeRate, newPlayer->wickets, newPlayer->economyRate);
        
        for(int j=0;j< teamCount;j++){
            if(strcmp(newPlayer->teamName, teamList[j].teamName) == 0){  
                int oldTotal = teamList[j].totalPlayers;
                float oldStrike = teamList[j].averageBattingStrikeRate * teamList[j].totalPlayers;

                teamList[j].totalPlayers += 1;
                if(newPlayer->role == Batsman || newPlayer->role == AllRounder){
                    teamList[j].averageBattingStrikeRate = (oldStrike + newPlayer->strikeRate)/teamList[j].totalPlayers;
                    
                }

                struct PlayerNode *newNode = (struct PlayerNode*)malloc(sizeof(struct PlayerNode));
            
                newNode->player = newPlayer;
                newNode->next = NULL;
                
                if(newPlayer->role == Batsman){
                    insertSorted(&teamList[j].batsmen, newNode);
                }else if(newPlayer->role == Bowler){
                    insertSorted(&teamList[j].bowlers, newNode);
                }else if(newPlayer->role == AllRounder){
                    insertSorted(&teamList[j].allRounders, newNode);
                }
                break;
            }
        }

    }
}

void addPlayerToTeam(){
    int tid;
    int pid;
    int low = 0, high = teamCount -1,mid=0;
    printf("Enter Team ID to add player: ");
    scanf("%d", &tid);
    if(tid < 1 || tid > teamCount){
        printf("Invalid Team ID\n");
        return;
    }
    while(low<=high){
        mid=low + (high - low)/2;
        if(teamList[mid].teamId == tid-1){
            int i = mid;
            if(teamList[i].totalPlayers >= MAX_PLAYERS){
                printf("Team %s already has maximum players (%d)\n", teamList[i].teamName, MAX_PLAYERS);
                return;
            }
            struct Players *newPlayer = (struct Players*)malloc(sizeof(struct Players));
            printf("Enter Player Details:\n");
            printf("Player ID: ");
            scanf("%d", &pid);
            if(isCorrectPID(pid)){
                newPlayer->playerId=pid;
            }
            else{
                free(newPlayer);
                return;
            }
            printf("Player Name: ");
            scanf(" %[^\n]s", newPlayer->playerName);
            strcpy(newPlayer->teamName, teamList[i].teamName);
            printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
            scanf("%d", &newPlayer->role);
            printf("Total Runs: ");
            scanf("%d", &newPlayer->totalRuns);
            printf("Batting Average: ");
            scanf("%f", &newPlayer->battingAverage);
            printf("Strike Rate: ");
            scanf("%f", &newPlayer->strikeRate);
            printf("Wickets: ");
            scanf("%d", &newPlayer->wickets);
            printf("Economy Rate: ");
            scanf("%f", &newPlayer->economyRate);
            newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer->role, newPlayer->battingAverage, newPlayer->strikeRate, newPlayer->wickets, newPlayer->economyRate);

            int oldTotal = teamList[i].totalPlayers;
            float oldStrike = teamList[i].averageBattingStrikeRate * teamList[i].totalPlayers;

            teamList[i].totalPlayers += 1;
            if(newPlayer->role == Batsman || newPlayer->role == AllRounder){
                teamList[i].averageBattingStrikeRate = (oldStrike + newPlayer->strikeRate)/teamList[i].totalPlayers;
                    
            }
            struct PlayerNode *newNode = (struct PlayerNode*)malloc(sizeof(struct PlayerNode));
            newNode->player = newPlayer;
            newNode->next = NULL;
            if(newPlayer->role == Batsman){
                insertSorted(&teamList[i].batsmen, newNode);
            }else if(newPlayer->role == Bowler){
                insertSorted(&teamList[i].bowlers, newNode);
            }else if(newPlayer->role == AllRounder){
                insertSorted(&teamList[i].allRounders, newNode);
            }
            printf("Player added successfully to Team %s\n", teamList[i].teamName);
            return;
        }
        else if(teamList[mid].teamId < tid-1){
            low = mid +1;
        } else {
            high = mid - 1;
        }
    }
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

void displayPlayersOfTeam(){
    int tid;
    int low = 0, high = teamCount -1,mid=0;
    printf("Enter Team ID: ");
    scanf("%d", &tid);
    if(tid < 1 || tid > teamCount){
        printf("Invalid Team ID\n");
        return;
    }
    while(low<=high){
        mid=low + (high - low)/2;
        if(teamList[mid].teamId == tid-1){
            int i = mid;
            printf("Players of Team %s:\n", teamList[i].teamName);
            printf("=============================================================================================\n");
            printf("ID\t%-25sRole\t\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n", "Name");
            printf("=============================================================================================\n");
            displayPlayerByRole(teamList[i].batsmen);
            displayPlayerByRole(teamList[i].bowlers);   
            displayPlayerByRole(teamList[i].allRounders);
            printf("=============================================================================================\n");
            printf("Total Players: %d\n", teamList[i].totalPlayers);
            printf("Average Batting Strike Rate: %.2f\n", teamList[i].averageBattingStrikeRate);
            return;
        }
        else if(teamList[mid].teamId < tid-1){
            low = mid +1;
        } else {
            high = mid - 1;
        }
    }
}

void displayTeamsByStrikeRate(){
    struct Teams temp[teamCount];
    for(int i=0;i< teamCount;i++){
        temp[i] = teamList[i];
    }
    for(int i=0;i< teamCount-1;i++){
        for(int j=0;j< teamCount-i-1;j++){
            if(temp[j].averageBattingStrikeRate < temp[j+1].averageBattingStrikeRate){
                struct Teams t = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = t;
            }
        }
    }
    printf("Teams sorted by Average Batting Strike Rate\n");
    printf("====================================================\n");
    printf("ID\t%-20sAvg Bat SR\tTotal Players\n", "Team Name");
    printf("====================================================\n");
    for(int i=0;i< teamCount;i++){
        printf("%d\t%-20s%.2f\t\t%d\n", i+1, temp[i].teamName, temp[i].averageBattingStrikeRate, temp[i].totalPlayers);
    }
    printf("====================================================\n");
}

void displayTopKPlayersByRole(){
    int tid, role, k;
    int low = 0, high = teamCount -1,mid=0;
    printf("Enter Team ID: ");
    scanf("%d", &tid);
    if(tid < 1 || tid > teamCount){
        printf("Invalid Team ID\n");
        return;
    }
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &role);
    printf("Enter number of top players to display: ");
    scanf("%d", &k);
    while(low<=high){
        mid=low + (high - low)/2;
        if(teamList[mid].teamId == tid-1){
            int i = mid;
            struct PlayerNode *temp;
            if(role == 1){
                temp = teamList[i].batsmen;
            }else if(role == 2){
                temp = teamList[i].bowlers;
            }else{
                temp = teamList[i].allRounders;
            }
            printf("Top %d Players of Team %s by Role %d:\n", k, teamList[i].teamName, role);
            printf("=============================================================================================\n");
            printf("ID\t%-25sRole\t\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n", "Name");
            printf("=============================================================================================\n");
            int count = 0;
            while(temp != NULL && count < k){
                printf("%d\t%-25s", temp->player->playerId, temp->player->playerName);
                if(role == 1){
                    printf("Batsman\t");
                }else if(role == 2){
                    printf("Bowler \t");
                }else{
                    printf("All-Rounder\t");
                }
                printf("%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->player->totalRuns, temp->player->battingAverage, temp->player->strikeRate, temp->player->wickets, temp->player->economyRate, temp->player->performanceIndex);
                temp = temp->next;
                count++;
            }
            printf("=============================================================================================\n");
            return;
        }
        else if(teamList[mid].teamId < tid-1){
            low = mid +1;
        } else {
            high = mid - 1;
        }
    }
}

void displayPlayersByRolePerformance(){
    int role;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &role);
    struct PlayerNode *allPlayers = NULL;
    for(int i=0;i< teamCount;i++){
        struct PlayerNode *temp;
        if(role == 1){
            temp = teamList[i].batsmen;
        }else if(role == 2){
            temp = teamList[i].bowlers;
        }else{
            temp = teamList[i].allRounders;
        }
        while(temp != NULL){
            struct PlayerNode *newNode = (struct PlayerNode*)malloc(sizeof(struct PlayerNode));
            newNode->player = temp->player;
            newNode->next = NULL;
            insertSorted(&allPlayers, newNode);
            temp = temp->next;
        }
    }
    if(role ==1){
        printf("Batsmen of all Teams:\n");
    }
    else if(role ==2){
        printf("Bowlers of all Teams:\n");
    }
    else{
        printf("All-Rounders of all Teams:\n");
    }
    printf("=============================================================================================\n");
    printf("ID\t%-25s%-15sRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n", "Name", "Team");
    printf("=============================================================================================\n");
    struct PlayerNode *temp = allPlayers;
    while(temp != NULL){
        printf("%d\t%-25s%-15s", temp->player->playerId, temp->player->playerName, temp->player->teamName);
            if(role == 1){
                printf("Batsman\t");
            }else if(role == 2){
                printf("Bowler \t");
            }else{
                printf("All-Rounder\t");
            }
            printf("%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->player->totalRuns, temp->player->battingAverage, temp->player->strikeRate, temp->player->wickets, temp->player->economyRate, temp->player->performanceIndex);
            temp = temp->next;
    }
    printf("=============================================================================================\n");
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
int main(){
    loadData();
    while(1){
        int choice;
        printf("\n============================================================\n");
        printf("       ICC ODI Player Performance Analyzer                    \n");
        printf("============================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("============================================================\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        
        switch(choice){
            case 1:
                addPlayerToTeam();
                break;
            case 2:
                displayPlayersOfTeam();
                break;
            case 3:
                displayTeamsByStrikeRate();
                break;
            case 4:
                displayTopKPlayersByRole();
                break;
            case 5:
                displayPlayersByRolePerformance();
                break;
            case 6:
                printf("Exiting the program....\n");
                freeMemory();
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    }
    return 0;
}