#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

#define TotalTeams 10
#define TotalPlayers 200

typedef struct MyPlayer {
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
    struct MyPlayer* next;
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
        team[index].name = teams[index];
        team[index].totalPlayers = 0;
        team[index].averageBattingStrikeRate = 0.0;
        team[index].playerHead = NULL;
    }

    // initialize player data
    for(int index = 0; index < TotalPlayers; index++) {
        Player playerData = players[index];

        MyPlayer* p = (MyPlayer*)malloc(sizeof(MyPlayer));
        p->id = playerData.id;
        p->name = playerData.name;
        p->team = playerData.team;
        p->role = playerData.role;
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

void updateTeamAverageStrikeRate(MyTeams* team) {
    float totalStrikeRate = 0.0;
    int count = 0; 

    MyPlayer* temp = team->playerHead;

    while(temp != NULL) {
        if(strcmp(temp->role, "Batsman") == 0 || strcmp(temp->role, "All-Rounder") == 0) {
            totalStrikeRate += temp->strikeRate;
            count++;
        }
        temp = temp->next;
    }

    team->averageBattingStrikeRate = (count > 0) ? totalStrikeRate / count : 0.0;
}

void addPlayerToTeam() {
    int teamId;
    printf("Enter Team ID To Add Player: ");
    scanf("%d", &teamId);

    if(teamId < 1 || teamId > TotalTeams) {
        printf("Invalid Team ID\n");
        return;
    }

    MyTeams* selectedTeam = &team[teamId - 1];

    MyPlayer *newPlayer = (MyPlayer *)malloc(sizeof(MyPlayer));
    if(!newPlayer) {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Enter Player Details: \n");

    printf("Player ID: ");
    scanf("%d", &newPlayer->id);
    getchar();

    char nameBuffer[50];
    printf("Name: ");
    fgets(nameBuffer, sizeof(nameBuffer), stdin);
    nameBuffer[strcspn(nameBuffer, "\n")] = '\0';
    newPlayer->name = strdup(nameBuffer);

    newPlayer->team = selectedTeam->name;

    int roleChoice;
    printf("Role (1: Batsman 2: Bowler 3: All-Rounder): ");
    scanf("%d", &roleChoice);

    char* role;

    if(roleChoice == 1) {
        role = "Batsman";
    } else if(roleChoice == 2) {
        role = "Bowler";
    } else if(roleChoice == 3) {
        role = "All-Rounder";
    } else {
        printf("Invalid role choice\n");
        free(newPlayer);
        return;
    }

    newPlayer->role = role;

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

    if (strcmp(newPlayer->role, "Batsman") == 0) {
        newPlayer->performanceIndex = (newPlayer->battingAverage * newPlayer->strikeRate) / 100;
    } else if (strcmp(newPlayer->role, "Bowler") == 0) {
        newPlayer->performanceIndex = (newPlayer->wickets * 2) + (100 - newPlayer->economyRate);
    } else {
        newPlayer->performanceIndex = ((newPlayer->battingAverage * newPlayer->strikeRate) / 100) + (newPlayer->wickets * 2);
    }

    newPlayer->next = selectedTeam->playerHead;
    selectedTeam->playerHead = newPlayer;
    selectedTeam->totalPlayers++;

    printf("Player added successfully to Team %s!\n", selectedTeam->name);
}

void displayPlayersOfSpecificTeam() {
    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    if(teamId < 1 || teamId > TotalTeams) {
        printf("Invalid Team ID\n");
        return;
    }

    MyTeams selectedTeam = team[teamId - 1];
    MyPlayer* current = selectedTeam.playerHead;

    printf("Players of Team %s:\n", selectedTeam.name);

    printf("=======================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=======================================================================================================\n");

    if(current == NULL) {
        printf("No players found for this team\n");
        return;
    }

    float totalStrikeRate = 0.0;
    int countForAvg = 0;

    while(current != NULL) {
        printf("%-5d %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n", current->id, current->name, current->role, current->totalRuns, current->battingAverage, current->strikeRate, current->wickets, current->economyRate, current->performanceIndex);

        current = current->next;
    }

    updateTeamAverageStrikeRate(&selectedTeam);

    printf("Total Players: %d\n", selectedTeam.totalPlayers);
    printf("Average Batting Strike Rate: %f\n", selectedTeam.averageBattingStrikeRate);
}

void displayTeamsByAverageBattingStrikeRate() {
   
}

void displayTopKPlayersOfASpecificTeamByRole() {

}

void displayAllPlayersOfAllTeams() {

}

void exitPerformanceAnalyzer() {

}

int main() {
    initializeData();

    while(1) {
        printf("=======================================================================================\n");
        printf("ICC ODI Player Performance Analyzer \n");
        printf("=======================================================================================\n");
        int choice;
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of Specific Role Across All Teams by Performance Index\n");
        printf("6. Exit\n");

        printf("=======================================================================================\n");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                addPlayerToTeam();
                break;
            case 2:
                displayPlayersOfSpecificTeam();
                break;
            case 3:
                displayTeamsByAverageBattingStrikeRate();
                break;
            case 4:
                displayTopKPlayersOfASpecificTeamByRole();
                break;
            case 5:
                displayAllPlayersOfAllTeams();
                break;
            case 6:
                exitPerformanceAnalyzer();
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}