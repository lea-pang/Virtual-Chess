/* Player.h : Header file for a chess player */
#ifndef PLAYER_H
#define PLAYER_H

/* insert necessary header files here */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* PLAYER struct */
typedef struct Player{
    char color;
    int num;
} PLAYER;

/* Function declarations */
PLAYER* createPlayer(char c, int n);

void deletePlayer(PLAYER* p);

void printPlayer(PLAYER* p);

char getPlayerColor(PLAYER* p);

int getPlayerNum(PLAYER* p);

void changePlayerColor(PLAYER* p);

void changePlayerNumber(PLAYER* p);

#endif

/* EOF*/

