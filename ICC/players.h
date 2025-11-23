#ifndef PLAYERS_H
#define PLAYERS_H

#include "Players_data.h"
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

int isCorrectPID(int pid);
float calculatePerformanceIndex(enum Role role, float battingAverage, float strikeRate, int wickets, float economyRate);
void insertSorted(struct PlayerNode **head, struct PlayerNode *newNode);
void displayPlayerByRole(struct PlayerNode *temp);
void freeMemory();

#endif