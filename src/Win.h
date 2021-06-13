/* Win.c : Header file for checking win conditions */
#ifndef WIN_H
#define WIN_H

#include "util.h"
#include <stdbool.h>
#include "Piece.h"
#include "Board.h"
#include "Player.h"
#include "Move.h"
#include "humanGame.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* strdup(const char* string);

//bool that checks if p1 is in check by p2, returns true if piece is in check otherwise false
PIECE* checkForCheck(BOARD* b, PLAYER* p1,PLAYER* p2);

//bool to see if p1 is in checkmate by p2
bool checkForCheckmate(BOARD* b, PLAYER* p1,PLAYER* p2);

//checks to see if the King is in check
bool kingCheckMove(char* checkLegalMoves, BOARD *b,PLAYER* p1,PLAYER* p2);

//prints out location of king
char* getLocationOfKing(BOARD* b, PLAYER* p);

//prints piece of enemy location
PIECE* getLocationOfEnemyPiece(BOARD* b, PLAYER* p,int x, int y);


//returns true if the player can make a move that'll put the other player in check
bool LCheck(BOARD *b, PLAYER* p1, PLAYER* p2);

//checks for stalemate/3 fold repetition
bool checkForSM(BOARD *b, PLAYER* p1, PLAYER *p2);

//checks for insufficient material
bool checkForIM(BOARD *b, PLAYER* p1, PLAYER* p2);
#endif
/*EOF*/
