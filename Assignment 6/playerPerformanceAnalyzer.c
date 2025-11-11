#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

#define TotalTeams 10
#define TotalPlayers 200

typedef struct {
    int id;
    const char *name;
    const char *team;
    const char *role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    MyPlayer* next;
} MyPlayer;

typedef struct {
    int id;
    const char* name;
    int totalPlayers;
    float averageBattingStrikeRate;
    MyPlayer* playerHead;
} MyTeams;

MyTeams team[TotalTeams];

void initializeData() {
    // initialize team data
    for(int index = 0; index < TotalTeams; index++) {
        team[index].id = index + 1;
        strcpy(team[index].name, teams[index]);
        team[index].totalPlayers = 0;
        team[index].averageBattingStrikeRate = 0.0;
        team[index].playerHead = NULL;
    }

    // initialize player data
    for(int index = 0; index < TotalPlayers; index++) {
        Player playerData = players[index];

        MyPlayer* p = (Player*)malloc(sizeof(Player));
        p->id = playerData.id;
        strcpy(p->name, playerData.name);
        strcpy(p->team, playerData.team);
        strcpy(p->role, playerData.role);
        p->totalRuns = playerData.totalRuns;
        p->battingAverage = playerData.battingAverage;
        p->strikeRate = playerData.strikeRate;
        p->wickets = playerData.wickets;
        p->economyRate = playerData.economyRate;
        p->next = NULL;

        // calculating performance index
        if(strcmp(p->role, "Batsman") == 0) {
            p->performanceIndex = (p->battingAverage * p->strikeRate) / 100;
        } else if(strcmp(p->role, "Bowler") == 0) {
            p->performanceIndex = (p->wickets * 2) + (100 - p->economyRate);
        } else {
            p->performanceIndex = ((p->battingAverage * p->strikeRate) / 100) + (p->wickets * 2);
        }

        for(int iterator = 0; iterator < TotalTeams; iterator++) {
            if(strcmp(team[iterator].name, p->team) == 0) {
                p->next = team[iterator].playerHead;
                team[iterator].playerHead = p;
                team[iterator].totalPlayers++;
                break;
            }
        }
    }
}