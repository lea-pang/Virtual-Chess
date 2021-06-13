/* Piece.c: C file for a chess piece */
/* Header files here */

#include "Piece.h"

/* Creates PIECE* */
PIECE* createPiece(char *t, char c, char l[3]){
	PIECE* p;
	p = malloc(sizeof(PIECE));
   memset(p->location,0,sizeof(char)*3);

	p->location[0] = l[0];
	p->location[1] = l[1];

	if (strcmp("pawn", t) == 0){
		p->type = PAWN;
		p->points = 1;
	}
	else if (strcmp("rook", t) == 0){
		p->type = ROOK;
		p->points = 5;
	}
	else if (strcmp("knight", t) == 0){
		p->type = KNIGHT;
		p->points = 3;
	}
	else if (strcmp("bishop", t) == 0){
		p->type = BISHOP;
		p->points = 3;
	}
	else if (strcmp("king", t) == 0){
		p->type = KING;
		p->points = 0;
	}
	else if (strcmp("queen", t) == 0){
		p->type = QUEEN;
		p->points = 9;
	}
	else
		return NULL;

	p->color = c;
	p->moves = 0;
	return p;
}

/* Deallocates memory for PIECE *p */
void deletePiece(PIECE *p){
	if (p != NULL){
		free(p);
      p = NULL;
	}
}

/* Gets the location of PIECE *p */
char* getLocation(PIECE *p){
	return p->location;
}

/* Gets the type of PIECE *p */
pieceType getType(PIECE *p){
	if (p != NULL)
		return p->type;
	return -1;
}

/* Gets the color of PIECE *p */
char getColor(PIECE *p){
	if (p != NULL)
		return p->color;
	return 'o';
}

/* Gets the number of moves made with PIECE *p */
int getMoves(PIECE *p){
	return p->moves;
}

/* Gets the number of points associated with PIECE *p */
int getPoints(PIECE *p){
	return p->points;
}

/* Changes the type of PIECE *p based on char* newtype (for pawn promotions) */
void changeType(PIECE* p, char* newtype){
	/* Assign new pieceType enum */
	if (strcmp("pawn", newtype) == 0)
		p->type = PAWN;
	else if (strcmp("rook", newtype) == 0)
		p->type = ROOK;
	else if (strcmp("knight", newtype) == 0)
		p->type = KNIGHT;
	else if (strcmp("bishop", newtype) == 0)
		p->type = BISHOP;
	else if (strcmp("king", newtype) == 0)
		p->type = KING;
	else if (strcmp("queen", newtype) == 0)
		p->type = QUEEN;
	else
		printf("\nCould not change piece type!\n");
}

/* Changes the current location of PIECE *p and increments the number of moves it has made */
void changeLocation(PIECE *p, char l[3]){
	p->location[0] = l[0];
	p->location[1] = l[1];
}

/* Adds to the move counter */
void addMove(PIECE *p){
	(p->moves)++;
}

/* Prints piece info */
void printPiece(PIECE *p){
	if (p == NULL){
		printf("No piece is here.\n");
	}

	else{
   		char type = ' ';
    	switch(p->type){
        	case 0:
            	type = 'P';
            	break;
        	case 1:
            	type = 'R';
            	break;
        	case 2:
            	type = 'N';
            	break;
        	case 3:
            	type = 'B';
            	break;
        	case 4:
            	type = 'K';
            	break;
        	case 5:
            	type = 'Q';
            	break;
        	default:
            	break;
    	}

		if (type != ' ')
    		printf("%c%c", getColor(p), type);
	}
}

/* EOF*/
