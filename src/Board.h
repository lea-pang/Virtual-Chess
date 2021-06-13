#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"

typedef struct Board{
   PIECE* gameBoard[8][8];
}BOARD;

void printBoard(BOARD* b);

BOARD* createBoard();

void resetBoard(BOARD* b);

PIECE *getPieceOnBoard(BOARD* b, char l[3]);

void deleteBoard(BOARD* b);

#endif
