/* Piece.h: Header file for a chess piece */
#ifndef PIECE_H
#define PIECE_H

/* Header files here */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PieceType enumeration */
typedef enum PieceType{PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN} pieceType;

/* PIECE struct */
typedef struct Piece{
	pieceType type;
	char color;
	char location[3];
	int moves;
	int points;
} PIECE;

/* Function declarations */
PIECE* createPiece(char *t, char c, char l[3]);

void deletePiece(PIECE *p);

char* getLocation(PIECE *p);

pieceType getType(PIECE *p);

char getColor(PIECE *p);

int getMoves(PIECE *p);

int getPoints(PIECE *p);

void addMove(PIECE *p);

void changeType(PIECE* p, char* newtype);

void changeLocation(PIECE* p, char l[3]);

void printPiece(PIECE *p);

#endif
/* EOF*/

