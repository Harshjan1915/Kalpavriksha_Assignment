#ifndef TEAMS_H
#define TEAMS_H

#include "players.h"

#define MAX_TEAMS 10
#define MAX_PLAYERS 50


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

void loadData();
void addPlayerToTeam();
void displayPlayersOfTeam();
void displayTeamsByStrikeRate();
void displayTopKPlayersByRole();
void displayPlayersByRolePerformance();

#endif