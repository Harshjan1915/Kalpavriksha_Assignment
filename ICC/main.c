#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "teams.h"
#include "players.h"



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