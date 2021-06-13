/* humanGame.h : Header file for human game */
#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "Piece.h"
#include "Move.h"
#include "Board.h"
#include "moveList.h"
#include "Win.h"
//withdraws a Move
void withdrawMove(PIECE* p,BOARD* b, MOVE* m, PIECE *pe);
//the human v human game
void Human_v_Human(PLAYER *p1, PLAYER *p2, BOARD *b);
//prompts the user if they want to declare a tie/draw
bool claimTie();
#endif
