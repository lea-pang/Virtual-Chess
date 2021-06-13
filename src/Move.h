/* Move.c : Header file for Move */
#ifndef MOVE_H
#define MOVE_H

/* annotated with // means not completely implemented yet */

#include <stdbool.h>
#include "Piece.h"
#include "Board.h"
#include "Player.h"
#include "util.h"

//MOVE struc which contains the locations of the input and points to the piece that's there
typedef struct move{
    PIECE *p;
    char start[3];
    char end[3];
} MOVE;

/*Allocates memory for a MOVE*/
MOVE* createMove(PIECE *piece, char l[5]);

/*Deallocates memory for a MOVE*/
void deleteMove(MOVE *m);

/*Returns the start location of a MOVE *m*/
char* getStart(MOVE *m);

/*Returns the end location of a MOVE *m */
char* getEnd(MOVE *m);

/*Returns the piece associated with MOVE *m */
PIECE *getPiece(MOVE *m);

/*Changes the current location of a PIECE *p to the end location of a MOVE *m*/
void changePieceLocation(MOVE *m, PIECE *p);

/*Prints information aobut MOVE *m*/
void printMove(MOVE *m);

/*Checks if the start location contains a same color piece as the player*/
bool validStartLoc(MOVE *m, BOARD *b, PLAYER *player);

/*Returns true if the end location of a potential move is empty on the board (for captures*/
bool emptySpace(MOVE *m, BOARD *b);

/*Checks if there is an enemy piece on the end location*/
bool enemySpace(PLAYER *player, PIECE *p, MOVE *m, BOARD *b);

/*the main move validty functions to be used in the game*/
bool validMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *player);

/*switch statement to decide if its a capture*/
bool captureMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *player);

/*updates the board with MOVE *m and input BOARD *b */
void updateBoard(MOVE *m, BOARD *b);

/*checks if a pawn move is valid*/
bool pawnMove(PIECE *p, MOVE *m, BOARD *b);

/*promotes a pawn, when allowed, based on user input*/
void promotionMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *player);

/*checks if a bishop move is valid*/
bool bishopMove(PIECE *p, MOVE *m, BOARD *b);

/*chekcs if a knight move is valid*/
bool knightMove(PIECE *p, MOVE *m, BOARD *b);

/*checks if a rook move is valid*/
bool rookMove(PIECE *p, MOVE *m, BOARD *b);

/*checks if a queen move is valid*/
bool queenMove(PIECE *p, MOVE *m, BOARD *b);

/*checks if a king move is valid*/
bool kingMove(PIECE *p, MOVE *m, BOARD *b);

/*Reverses a capture when a player withdraws a move*/
void reverseCapture(PIECE *p, BOARD *b, char l[3]);

/*checks if a capture by a pawn is valid*/
bool pawnCapture(PIECE *p, MOVE *m, BOARD *b);

/*checks if a capture by a bishop is valid*/
bool bishopCapture(PIECE *p, MOVE *m, BOARD *b);

/*checks if capture by a knight is valid*/
bool knightCapture(PIECE *p, MOVE *m, BOARD *b);

/*checks if capture by a queen is valid*/
bool queenCapture(PIECE *p, MOVE *m, BOARD *b);

/*checks if capture by a king is valid*/
bool kingCapture(PIECE *p, MOVE *m, BOARD *b);

/*checks if capture by a rook is valid*/
bool rookCapture(PIECE *p, MOVE *m, BOARD *b);

/*Gets the board array x variable based on location char* c from move*/
int getX(char c[3]);

/*Gets the board array y variable based on location char* c from move*/
int getY(char c[3]);

/*checks conditions for a pawn's en passant*/
bool enPassant(PIECE *p, MOVE *m, BOARD *b);

/*captures enemy pawn if en passant is true*/
void enPassantCapture(PIECE *p, MOVE *m, BOARD *b);

/*checks if king can make a castling move*/

bool castlingCondition(PIECE *k, MOVE *m, BOARD *b);

/*changes the position of the rook is castling is valid*/
void castling(MOVE *m, BOARD *b);

/*validates user move input*/
char* moveInput(BOARD *b);

#endif
/* EOF */
