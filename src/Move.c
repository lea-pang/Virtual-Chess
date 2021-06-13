/* Move.c: C file for Move */
#include "Move.h"
#include "moveList.h"
/* Allocates memory for a MOVE * */
MOVE* createMove(PIECE *piece, char l[5]){
    MOVE *m = malloc(sizeof(MOVE));
   
	memset(m, 0, (sizeof(MOVE))); 
	m->p = piece;

	m->start[0] = l[0];
	m->start[1] = l[1];
	m->end[0] = l[2];
	m->end[1] = l[3];

    return m;
}

/* Deallocates memory for a MOVE* */
void deleteMove(MOVE *m){
	if (m != NULL){
		free(m);
	}
}

/* Returns the start location of MOVE *m */
char* getStart(MOVE *m){
	if (m!= NULL){
		return m->start;
	}
	return "NN";
}

/* Returns the end location of MOVE *m */
char* getEnd(MOVE *m){
	if (m!= NULL){
		return m->end;
	}
	return "NN";
}

/* Returns the piece associated with MOVE *m */
PIECE* getPiece(MOVE *m){
	if (m != NULL){
		return m->p;
	}
	return NULL;
}

/* Changes the current location of PIECE *p to the end location of MOVE *m */
void changePieceLocation(MOVE *m, PIECE *p){
	changeLocation(p, m->end);
}

/* Prints information about MOVE *m */
void printMove(MOVE *m){
	if (m != NULL){
		printf("%c%d", m->p->color, m->p->type);
    	printf("%s%s", m->start, m->end);
	}
	else{
		printf("\nCould not print move!\n");
	}
}

/* Checks if the end location of a potential move is empty on the board (for captures)*/
bool emptySpace(MOVE *m, BOARD *b)
{
	/* Location contains no piece */
	if (getPieceOnBoard(b, m->end) == NULL)
		return true;
	
	/* Location contains a piece */
	else
		return false;
}

/* Returns true if the player tries to move the other player's piece */
bool enemySpace(PLAYER *player, PIECE *p,  MOVE *m, BOARD *b)
{
	/*if player is white, checks if black piece on end location*/
	if ((getPlayerColor(player)  == 'w') && (getColor(p) == 'b'))
		return true;
	/*if player is black, checks if white piece on end location*/
	else if ((getPlayerColor(player) == 'b') && (getColor(p) == 'w'))
		return true;
	/*if NULL or piece is same color  as player,*/ 
	else 
		return false;
}
/* Checks if the start location contains a same color piece as the player */
bool validStartLoc(MOVE *m, BOARD *b, PLAYER *player)
{
	/* Location contains a piece */
	if (getPieceOnBoard(b, m->start) != NULL){

		/* Location contains a same color piece */
		if (getColor(getPieceOnBoard(b, m->start)) == getPlayerColor(player))
			return true;

		/* Location contains the other player's piece */
		else
			return false;
	}
	
	/* Location does not contain a piece */
	else
		return false;
}

/* The main move validity function to be used in the game (not completely implemented)*/
bool validMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *player)
{
	/* Some piece moves return true by default, since not fully implemented */

	/* Checks if the start location is valid */
	if (validStartLoc(m, b, player)){

		/* Switch for piece type movement checking */
		switch (getType(getPiece(m))){
			case PAWN:
				return pawnMove(p, m, b);
				break;	
			case ROOK:
				return rookMove(p, m, b);
				break;
			case KNIGHT:
				return knightMove(p, m, b);
				break;
			case BISHOP:
				return bishopMove(p, m, b);
				break;
			case KING:
				return kingMove(p, m, b);
				break;
			case QUEEN:
				return queenMove(p, m, b);
				break;
			default:
				break;
		}
	}
	return false;
}

//switch statement to decide if it's a capture
bool captureMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *player)
{
	/* Checks if the start location is valid */
	if (validStartLoc(m, b, player)){

		/* Switch for piece type movement for a capture checking */
		switch (getType(getPiece(m))){
			case PAWN:
				return pawnCapture(p, m, b);
				break;	
			case ROOK:
				return rookCapture(p, m, b);
				break;
			case KNIGHT:
				return knightCapture(p, m, b);
				break;
			case BISHOP:
				return bishopCapture(p, m, b);
				break;
			case KING:
				return kingCapture(p, m, b);
				break;
			case QUEEN:
				return queenCapture(p, m, b);
				break;
			default:
				break;
		}
	}
	return false;
}

/* Updates the board with MOVE *m and input BOARD *b */
void updateBoard(MOVE *m, BOARD *b)
{
	int endcol = getEnd(m)[0] - 65;
	int endrow = 8 - (getEnd(m)[1] - 48);
	
	int startcol = getStart(m)[0] - 65;
	int startrow = 8 - (getStart(m)[1] - 48);

	b->gameBoard[startrow][startcol] = NULL;
	
	b->gameBoard[endrow][endcol] = getPiece(m);
	
}

/* Reverses a capture when a player withdraws a move */
void reverseCapture(PIECE *p, BOARD *b, char l[3]){
	int x = getX(l);
	int y = getY(l);

	b->gameBoard[y][x] = p;
}

/*Gets the board array x variable based on location char* c from move*/
int getX(char c[3])
{
	char x = c[0];
	if (x == 'A')
		return 0;
	else if (x == 'B')
		return 1;
	else if (x == 'C')
		return 2;
	else if (x == 'D')
		return 3;
	else if (x == 'E')
		return 4;
	else if (x == 'F')
		return 5;
	else if (x == 'G')
		return 6;
	else if (x == 'H')
		return 7;
	else
		return 8; // invalid input
}

/* Gets the board array y variable based on location char* c from move*/
int getY(char c[3])
{
	char y = c[1];

	if (y == '1')
		return 7;
	else if (y == '2')
		return 6;
	else if (y == '3')
		return 5;
	else if (y == '4')
		return 4;
	else if (y == '5')
		return 3;
	else if (y == '6')
		return 2;
	else if (y == '7')
		return 1;
	else if (y == '8')
		return 0;
	else
		return 8; // invalid input
}

/* Validates move input */
char* moveInput(BOARD *b){
	char input[30] = "";
	char start[3] = "";
	char* user_input = NULL;
   user_input = input;

    while (true){
		scanf("%s", input);

        if (strcmp(input, "exit") == 0){ // player wants to exit
       	    return strdup(user_input);
        }
        
        if ( (input[0] < 65) || (input[0] > 72) ){ // capital letter
            printf("\nPlease input a valid move: ");
			input[0] = '\0';
            continue;
        }
        
        if ( (input[1] < 49 ) || (input[1] > 56) ){ // number
            printf("\nPlease input a valid move: ");
			input[0] = '\0';
            continue;
        }
        
        if ( (input[2] < 65 ) || (input[2] > 72) ){ // capital letter
            printf("\nPlease input a valid move: ");
			input[0] = '\0';
            continue;
        }
        
        if ( (input[3] < 49 ) || (input[3] > 56) ){ // number
            printf("\nPlease input a valid move: ");
			input[0] = '\0';
            continue;
        }
        
		if (strlen(input) > 4){
			printf("\nPlease input a valid move: ");
			input[0] = '\0';
			continue;
		}

		start[0] = input[0];
		start[1] = input[1];

		if (getPieceOnBoard(b, start) == NULL){
			printf("\nThere is no piece there.\n\nPlease input a valid move: ");
			input[0] = '\0';
			start[0] = '\0';
			continue;
		}

    	break; // input is valid, break out of loop
    } 

    return strdup(user_input);
}

/* Checks if a pawn's move is valid */
bool pawnMove(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	/* Check if it is the first move for the piece*/
	if ((getMoves(p)==0) && (getColor(p)=='w') && (xs==xe) && (ye==(ys-2)) && (b->gameBoard[ye][xe]==NULL) && (b->gameBoard[ys-1][xe]==NULL)){
		return true;
	}

	else if ((getMoves(p)==0) && (getColor(p)=='b') && (xs==xe) && (ye==(ys+2)) && (b->gameBoard[ye][xe]==NULL) && (b->gameBoard[ys+1][xe]==NULL)){
		return true;
	}

	/* Pawn has already moved once */
	else if ((getColor(p)=='w') && (xs==xe) && (ye==(ys-1)) && (b->gameBoard[ye][xe]== NULL)){
		return true;
	}

	else if ((getColor(p)=='b') && (xs==xe) && (ye==(ys+1)) && (b->gameBoard[ye][xe]==NULL)){
		return true;
	}
	else if ((getColor(p)=='w') && (((xe==(xs-1)) && (ye==(ys-1))) || ((xe==(xs+1))&&(ye==(ys-1)))))	
		if (enPassant(p, m, b)) {
			return true; }
		else
			return false;
	else if ((getColor(p)=='b') && (((xe==(xs+1)) && (ye==(ys+1)))|| ((xe==(xs-1))&&(ye==(ys+1)))))
		if (enPassant(p, m, b)) {
			return true; }
		else
			return false;
	else
		return false;
}

/* Checks if a capture by a pawn is valid */
bool pawnCapture(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	/* Capture forward diagonally */
	if ((getColor(p)=='w') && (((xe==(xs-1)) && (ye==(ys-1))) || ((xe==(xs+1))&&(ye==(ys-1)))))	
		return true;
	else if ((getColor(p)=='b') && (((xe==(xs+1)) && (ye==(ys+1)))|| ((xe==(xs-1))&&(ye==(ys+1)))))
		return true;
	else
		return false;
}
/*checks conditions for en passant*/
bool enPassant(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	//int enemyX;
	if (getType(getPiece(m))!=PAWN)
		return false;
	if (!((((getColor(p)=='w') && (((xe==(xs-1)) && (ye==(ys-1))) || ((xe==(xs+1))&&(ye==(ys-1))))))||((getColor(p)=='b') && (((xe==(xs+1)) && (ye==(ys+1)))|| ((xe==(xs-1))&&(ye==(ys+1)))))))
		{	
		return false;}

	/*make sure end location is empty*/
	if (b->gameBoard[ye][xe]!=NULL)
		return false;
	/*get piece that will be captured*/
	//PIECE *ep;
	char color;
	if ((getColor(p)=='w') && ((xe==(xs-1)) && (ye==(ys-1)))) /*northwest*/
		{color = getColor(b->gameBoard[ys][xs-1]); /*west*/
		//enemyX = xs-1;
		}
	else if ((getColor(p)=='w') && ((xe==(xs+1)) && (ye==(ys-1)))) /*northeast*/
		{color = getColor(b->gameBoard[ys][xs+1]); /*east*/
		//enemyX = xs+1;
		 }
	else if ((getColor(p)=='b') && ((xe==(xs-1)) && (ye==(ys+1)))) /*southwest*/
		{color = getColor(b->gameBoard[ys][xs-1]); /*west*/
		//enemyX = xs-1; 
		}
	else if ((getColor(p)=='b') && ((xe==(xs+1)) && (ye==(ys+1)))) /*southeast*/
		{color = getColor(b->gameBoard[ys][xs+1]); /*east*/
		//enemyX = xs+1;
		}
	else
		color = ' ';
	/*make sure that it is the enemy piece*/
	if (((getColor(p)=='w') && (color!='b')) || ((getColor(p)=='b') && (color!='w')))
		return false;
	/*check if LAST move was the enemy pawn moving 2 pieces ahead*/
	//MOVE *lm;
	//lm = getTail()->move;
	char* lstart = getStart(getTail()->move);
	char* lend = getEnd(getTail()->move);
	int lxs = getX(lstart);
	int lys = getY(lstart);
	int lxe = getX(lend);
	int lye = getY(lend);
	if ((color=='w') && (lys==6) && (lye==4) && (lxs==lxe)) {	
		//b->gameBoard[ys][enemyX] = NULL;
		//deletePiece(ep);
		//deleteMove(lm);
		return true; }
	else if ((color=='b') && (lys==1) && (lye==3) && (lxs==lxe)) {
		//b->gameBoard[ys][enemyX] = NULL;
		//deletePiece(ep);
		//deleteMove(lm);
		return true; }
	return false;	
}

/*captures enemy pawn if en passant is true*/
void enPassantCapture(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	int enemyX;

	/*get piece that will be captured*/
	//PIECE *ep;
	if ((getColor(p)=='w') && ((xe==(xs-1)) && (ye==(ys-1)))) /*northwest*/
		{//ep = b->gameBoard[ys][xs-1]; /*west*/
		enemyX = xs-1;}
	else if ((getColor(p)=='w') && ((xe==(xs+1)) && (ye==(ys-1)))) /*northeast*/
		{//ep = b->gameBoard[ys][xs+1]; /*east*/
		enemyX = xs+1; }
	else if ((getColor(p)=='b') && ((xe==(xs-1)) && (ye==(ys+1)))) /*southwest*/
		{//ep = b->gameBoard[ys][xs-1]; /*west*/
		enemyX = xs-1; }
	else if ((getColor(p)=='b') && ((xe==(xs+1)) && (ye==(ys+1)))) /*southeast*/
		{//ep = b->gameBoard[ys][xs+1]; /*east*/
		enemyX = xs+1; }
	else
		enemyX = 0;
	deletePiece(b->gameBoard[ys][enemyX]);
	b->gameBoard[ys][enemyX] = NULL;

}
/* Promotes a pawn, when allowed, based on user input */
void promotionMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *player)
{
	if ((getType(p) == PAWN) && ( validMove(p, m, b, player) || captureMove(p, m, b, player))){
		// Stores the end location and its y-coordinate
		char* end = getEnd(m);
		int y = getY(end);
		char newtype[7] = "";
	
		// If a pawn is at the opposite side of the board, ask for promotion and change piece type
		if((getColor(p) == 'w') && (y == 0)) // Player 1's pawn
		{
			printf("Player 1, your pawn is allowed to promote! Enter a piece type to promote this pawn to (other than king): ");
			scanf("%s", newtype);
			changeType(p, newtype);
		}

		if ((getColor(p) == 'b') && (y == 7)) // Player 2's pawn
		{
			printf("Player 2, your pawn is allowed to promote! Enter a piece type to promote this pawn to (other than king): ");
			scanf("%s", newtype);
			changeType(p, newtype);
		}
	}
}

/* Checks if a bishop's move is valid */
bool bishopMove(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);

	/*Checks spaces between start and end location*/
	int difx = xe-xs; /* - if end is west of start, + if end is east of start*/
	int dify = ye-ys; /* - if end is north of start, + if end is south of start*/
	
	/*Check if moving diagonally*/
	if ((difx != dify) && (difx != (-1*dify))) {
		return false;}

	/*Check if pieces are in the way*/
	int i;
	if (dify<0 && difx<0) /*diagonal: northwest*/
		for (i=1;i<=(-1*difx);i=i+1) {
			if (b->gameBoard[ys-i][xs-i] == NULL)
				{
				continue;}
			else {
				return false;}
		} 

	else if (dify<0 && difx>0) /*diagonal: northeast*/
		for(i=1; i<=difx; i=i+1) {
			if (b->gameBoard[ys-i][xs+i] == NULL)
				{
				continue;}
			else {	
				return false;}
		}
	else if (dify>0 && difx<0) /*diagonal: southwest */
		for (i=1; i<=dify;i=i+1) {
			if (b->gameBoard[ys+i][xs-i] == NULL)
				{
				continue;}
			else { 
				return false; }
		}
	else if (dify>0 && difx>0) /*diagonal: southeast*/
		for (i=1; i<=difx; i=i+1) {
			if (b->gameBoard[ys+i][xs+i] == NULL)
				{
				continue;}
			else {
				return false;}		
		}

	else
		return false;

	return true;
}

/* Checks if a capture by a bishop is valid */
bool bishopCapture(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);

	/*Checks spaces between start and end location*/
	int difx = xe-xs; /* - if end is west of start, + if end is east of start*/
	int dify = ye-ys; /* - if end is north of start, + if end is south of start*/
	
	/*check if moving diagonally*/
	if ((difx != dify) && (difx != (-1*dify))) {
		return false;}

	/*check if pieces are in the way*/
	int i;
	if (dify<0 && difx<0) /*diagonal: northwest*/
		for (i=1;i<(-1*difx);i=i+1) {
			if (b->gameBoard[ys-i][xs-i] == NULL)
				{
				continue;}
			else {
				return false;}
		} 

	else if (dify<0 && difx>0) /*diagonal: northeast*/
		for(i=1; i<difx; i=i+1) {
			if (b->gameBoard[ys-i][xs+i] == NULL)
				{
				continue;}
			else {	
				return false;}
		}
	else if (dify>0 && difx<0) /*diagonal: southwest */
		for (i=1; i<dify;i=i+1) {
			if (b->gameBoard[ys+i][xs-i] == NULL)
				{
				continue;}
			else { 
				return false; }
		}
	else if (dify>0 && difx>0) /*diagonal: southeast*/
		for (i=1; i<difx; i=i+1) {
			if (b->gameBoard[ys+i][xs+i] == NULL)
				{
				continue;}
			else {
				return false;}		
		}

	else
		return false;

	return true;
}

/* Checks if a knight's move is valid */
bool knightMove(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	if ((xe==(xs-2)) && (ye==(ys-1)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs-1))&&(ye==(ys-2)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs+1))&&(ye==(ys-2)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs+2))&&(ye==(ys-1)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs+2))&&(ye==(ys+1)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs+1))&&(ye==(ys+2)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs-1))&&(ye==(ys+2)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else if((xe==(xs-2))&&(ye==(ys+1)) && (b->gameBoard[ye][xe] == NULL))
			return true;
	else
		return false;
}

/* Checks if capture by a knight is valid */
bool knightCapture(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	if ((xe==(xs-2)) && (ye==(ys-1)))
			return true;
	else if((xe==(xs-1))&&(ye==(ys-2)))
			return true;
	else if((xe==(xs+1))&&(ye==(ys-2)))
			return true;
	else if((xe==(xs+2))&&(ye==(ys-1)))
			return true;
	else if((xe==(xs+2))&&(ye==(ys+1)))
			return true;
	else if((xe==(xs+1))&&(ye==(ys+2)))
			return true;
	else if((xe==(xs-1))&&(ye==(ys+2)))
			return true;
	else if((xe==(xs-2))&&(ye==(ys+1)))
			return true;
	else
		return false;
}

/* Checks if a rook's move is valid */
bool rookMove(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	/*checks spaces between start and end location*/
	int difx = xe-xs; /* - if end is west of start, + if end is east of start*/
	int dify = ye-ys; /* - if end is north of start, + if end is south of start*/
	/*checks if moving in same row or same column*/
	if ((xs!=xe) && (ys!=ye)) {
		return false;}

	/*checks if pieces are in the way*/
	int i;
	if ((xs==xe) && (ys>ye))/*north*/
		for (i=1; i<=(-1*dify);i=i+1) {
			if (b->gameBoard[ys-i][xs] == NULL)
				{
				continue;}
			else
				return false;
		}
	else if ((xs<xe) && (ys==ye))/*east*/
		for (i=1; i<=difx; i=i+1) {
			if (b->gameBoard[ys][xs+i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs==xe) && (ys<ye)) /*south*/
		for (i=1; i<=dify; i=i+1) {
			if (b->gameBoard[ys+i][xs] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs>xe) && (ys==ye))/*west*/
		for (i=1; i<=(-1*difx); i=i+1) {
			if (b->gameBoard[ys][xs-i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else
		{
		return false;}
	return true;
}

/* Checks if capture by a rook is valid */
bool rookCapture(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	/*checks spaces between start and end location*/
	int difx = xe-xs; /* - if end is west of start, + if end is east of start*/
	int dify = ye-ys; /* - if end is north of start, + if end is south of start*/
	/*checks if moving in same row or same column*/
	if ((xs!=xe) && (ys!=ye)) {
		return false;}

	/*checks if pieces are in the way*/
	int i;
	if ((xs==xe) && (ys>ye))/*north*/
		for (i=1; i<(-1*dify);i=i+1) {
			if (b->gameBoard[ys-i][xs] == NULL)
				{
				continue;}
			else
				return false;
		}
	else if ((xs<xe) && (ys==ye))/*east*/
		for (i=1; i<difx; i=i+1) {
			if (b->gameBoard[ys][xs+i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs==xe) && (ys<ye)) /*south*/
		for (i=1; i<dify; i=i+1) {
			if (b->gameBoard[ys+i][xs] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs>xe) && (ys==ye))/*west*/
		for (i=1; i<(-1*difx); i=i+1) {
			if (b->gameBoard[ys][xs-i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else
		{
		return false;}
	return true;
}

/* Checks if a queen's move is valid */
bool queenMove(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	/*checks spaces between start and end location*/
	int difx = xe-xs; /* - if end is west of start, + if end is east of start*/
	int dify = ye-ys; /* - if end is north of start, + if end is south of start*/
	
	/*checks if moving in same row or column AND if it is moving diagonally*/
	if (((xs!=xe) && (ys!=ye)) && ((difx != dify)&&(difx != (-1*dify)))) {
		return false;}

	/*checks if pieces are in the way horizontally/vertically*/
	int i;
	if ((xs==xe) && (ys>ye))/*north*/
		for (i=1; i<=(-1*dify);i=i+1) {
			if (b->gameBoard[ys-i][xs] == NULL)
				{
				continue;}
			else
				return false;
		}
	else if ((xs<xe) && (ys==ye))/*east*/
		for (i=1; i<=difx; i=i+1) {
			if (b->gameBoard[ys][xs+i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs==xe) && (ys<ye)) /*south*/
		for (i=1; i<=dify; i=i+1) {
			if (b->gameBoard[ys+i][xs] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs>xe) && (ys==ye))/*west*/
		for (i=1; i<=(-1*difx); i=i+1) {
			if (b->gameBoard[ys][xs-i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}

	/*check if pieces are in the way diagonally*/
	else if (dify<0 && difx<0) /*diagonal: northwest*/
		for (i=1;i<=(-1*difx);i=i+1) {
			if (b->gameBoard[ys-i][xs-i] == NULL)
				{
				continue;}
			else {
				return false;}
		} 

	else if (dify<0 && difx>0) /*diagonal: northeast*/
		for(i=1; i<=difx; i=i+1) {
			if (b->gameBoard[ys-i][xs+i] == NULL)
				{
				continue;}
			else {
				return false;}
		}
	else if (dify>0 && difx<0) /*diagonal: southwest */
		for (i=1; i<=dify;i=i+1) {
			if (b->gameBoard[ys+i][xs-i] == NULL)
				{
				continue;}
			else {
				return false; }
		}
	else if (dify>0 && difx>0) /*diagonal: southeast*/
		for (i=1; i<=difx; i=i+1) {
			if (b->gameBoard[ys+i][xs+i] == NULL)
				{
				continue;}
			else {
				return false;}		
		}

	else
		return false;
	return true;
}

/* Checks if capture by a queen is valid */
bool queenCapture(PIECE *p, MOVE *m, BOARD *b)
{

	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);
	
	/*checks spaces between start and end location*/
	int difx = xe-xs; /* - if end is west of start, + if end is east of start*/
	int dify = ye-ys; /* - if end is north of start, + if end is south of start*/
	
	/*checks if moving in same row or column AND if it is moving diagonally*/
	if (((xs!=xe) && (ys!=ye)) && ((difx != dify)&&(difx != (-1*dify)))) {
		return false;}

	/*checks if pieces are in the way horizontally/vertically*/
	int i;
	if ((xs==xe) && (ys>ye))/*north*/
		for (i=1; i<(-1*dify);i=i+1) {
			if (b->gameBoard[ys-i][xs] == NULL)
				{
				continue;}
			else
				return false;
		}
	else if ((xs<xe) && (ys==ye))/*east*/
		for (i=1; i<difx; i=i+1) {
			if (b->gameBoard[ys][xs+i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs==xe) && (ys<ye)) /*south*/
		for (i=1; i<dify; i=i+1) {
			if (b->gameBoard[ys+i][xs] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}
	else if ((xs>xe) && (ys==ye))/*west*/
		for (i=1; i<(-1*difx); i=i+1) {
			if (b->gameBoard[ys][xs-i] == NULL)
				{
				continue;}
			else
				{
				return false;}
		}

	/*check if pieces are in the way diagonally*/
	else if (dify<0 && difx<0) /*diagonal: northwest*/
		for (i=1;i<(-1*difx);i=i+1) {
			if (b->gameBoard[ys-i][xs-i] == NULL)
				{
				continue;}
			else {
				return false;}
		} 

	else if (dify<0 && difx>0) /*diagonal: northeast*/
		for(i=1; i<difx; i=i+1) {
			if (b->gameBoard[ys-i][xs+i] == NULL)
				{
				continue;}
			else {	
				return false;}
		}
	else if (dify>0 && difx<0) /*diagonal: southwest */
		for (i=1; i<dify;i=i+1) {
			if (b->gameBoard[ys+i][xs-i] == NULL)
				{
				continue;}
			else {
				return false; }
		}
	else if (dify>0 && difx>0) /*diagonal: southeast*/
		for (i=1; i<difx; i=i+1) {
			if (b->gameBoard[ys+i][xs+i] == NULL)
				{
				continue;}
			else {
				return false;}		
		}

	else
		return false;
	return true;
}

/* Checks if a king's move is valid */
bool kingMove(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);

	/*check 8 possible positions for king*/
	if ((xe==(xs-1)) && (ye==(ys-1)) && b->gameBoard[ye][xe]==NULL)
		return true; /*top left*/
	else if ((xe==xs) && (ye==(ys-1)) && b->gameBoard[ye][xe]==NULL)
		return true; /*top middle*/
	else if ((xe==(xs+1)) && (ye==(ys-1)) && b->gameBoard[ye][xe]==NULL)
		return true; /*top right*/
	else if ((xe==(xs-1)) && (ye==ys) && b->gameBoard[ye][xe]==NULL)
		return true; /*center left*/
	else if ((xe==(xs+1)) && (ye==ys) && b->gameBoard[ye][xe]==NULL)
		return true; /*center right*/
	else if ((xe==(xs-1)) && (ye==(ys+1)) && b->gameBoard[ye][xe]==NULL)
		return true; /*bottom left*/
	else if ((xe==xs) && (ye==(ys+1)) && b->gameBoard[ye][xe]==NULL)
		return true; /*bottom middle*/
	else if ((xe==(xs+1)) && (ye==(ys+1)) && b->gameBoard[ye][xe]==NULL)
		return true; /*bottom right*/

	else if (castlingCondition(p, m, b))
		return true; /*castling*/
	else
		return false;
}

/* Checks if capture by a king is valid */
bool kingCapture(PIECE *p, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int xs = getX(start);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);

	/*check 8 possible positions for king*/
	if ((xe==(xs-1)) && (ye==(ys-1)))
		return true; /*top left*/
	else if ((xe==xs) && (ye==(ys-1)))
		return true; /*top middle*/
	else if ((xe==(xs+1)) && (ye==(ys-1)))
		return true; /*top right*/
	else if ((xe==(xs-1)) && (ye==ys))
		return true; /*center left*/
	else if ((xe==(xs+1)) && (ye==ys))
		return true; /*center right*/
	else if ((xe==(xs-1)) && (ye==(ys+1)))
		return true; /*bottom left*/
	else if ((xe==xs) && (ye==(ys+1)))
		return true; /*bottom middle*/
	else if ((xe==(xs+1)) && (ye==(ys+1)))
		return true; /*bottom right*/
	else
		return false;

}

/*checks if king can make a castling move*/
bool castlingCondition (PIECE *k, MOVE *m, BOARD *b)
{
	char* start = getStart(m);
	char* end = getEnd(m);
	int ys = getY(start);
	int xe = getX(end);
	int ye = getY(end);

	if (getType(getPiece(m))!=KING)
		return false;
	if (getMoves(k)!=0) /*checks that king has not made any moves*/
		return false;
	if (ys!=ye) /*makes sure that king is moving horizontally*/
		return false;
	if ((xe!=2) && (xe!=6)) /*makes sure king is moving 2 spaces horizontally*/
		return false;

	if (xe==2) { /*castling to the left*/
		if ((b->gameBoard[ye][1]!=NULL) || (b->gameBoard[ye][2]!=NULL) || (b->gameBoard[ye][3]!=NULL)) /*makes sure spaces are empty*/
			return false;
		//PIECE *r; /*check conditions for rook*/
		//r = b->gameBoard[ye][0];
		if (getType(b->gameBoard[ye][0])!=ROOK) { /*makes sure piece is a rook*/
			//deletePiece(r);
			return false; }
		if (getMoves(b->gameBoard[ye][0])!=0) { /*makes sure rook has not moved before*/
			//deletePiece(r);
			return false; }
		//deletePiece(r);
		return true;
	}
	else if (xe==6) { /*castling to the right*/
		if ((b->gameBoard[ye][6]!=NULL) || (b->gameBoard[ye][5]!=NULL))
			return false; 
		//PIECE *r; /*checks conditions for rook*/
		//r = b->gameBoard[ye][7];
		if (getType(b->gameBoard[ye][7])!=ROOK) { /*makes sure piece is a rook */
			//deletePiece(r);
			return false; }
		if (getMoves(b->gameBoard[ye][7])!=0) { /*makes sure rook has not moved before*/
			//deletePiece(r);
			return false; }
		//deletePiece(r);
		return true;
	}
	else
		return false;
		
	
}

/*changes the position of the rook if castling is valid*/
void castling (MOVE *m, BOARD *b) {
	char* end = getEnd(m);
	int xe = getX(end);
	int ye = getY(end);

	char rookMove[5] = ""; /*string defining Move*/
	PIECE *r; /*takes rook to move*/
	if (xe == 2) /*castling to the left*/ {
		r = b->gameBoard[ye][0];
		rookMove[2] = 'D';
		}
	
	else if (xe == 6) /*castling to the right*/ {
		r = b->gameBoard[ye][7];
		rookMove[2] = 'F';
		}
	
	if (getColor(r)=='w')
		rookMove[3] = '1';
	else if (getColor(r)=='b')
		rookMove[3] = '8';

	strncpy(rookMove, getLocation(r), 2);
	MOVE *mR = createMove(r, rookMove);

	/* changes rook position */
	changePieceLocation (mR, r);
	updateBoard(mR, b);
}
