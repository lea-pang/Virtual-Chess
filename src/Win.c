#include "Win.h"
#include "AIgame.h"
#include "moveList.h"
#include "Board.h"

/*Checks if player 1 (passed through as parameter) is in check by player 2*/
PIECE* checkForCheck(BOARD* b,PLAYER* p1,PLAYER* p2){
   int x = 0;
   int y = 0;
   char start[2] = "";
   char end[3] = "";
   char movLocation[5] = "";
   char* endLoc = NULL;
   PIECE* piecePointer;
    
   for(x = 0; x < 8; x++){
      for(y = 0;y < 8; y++){
         /*arbitrary piece pointer that is set to enemy piece or null(if there is no piece there) to parse through all enemy pieces*/
         piecePointer = getLocationOfEnemyPiece(b,p2,x,y);
         
         /*makes sure there is a piece at the location*/
         if(piecePointer != NULL){
          
            /*Make a move to represent a possible king take, starting at current location of a piece*/
			start[0] = (piecePointer->location)[0];
			start[1] = (piecePointer->location)[1];

            /*end is set to the current location of the opposite king*/
         endLoc = getLocationOfKing(b,p1);
         end[0] = endLoc[0];
         end[1] = endLoc[1];
         free(endLoc);

			movLocation[0] = start[0];
			movLocation[1] = start[1];
			
			movLocation[2] = end[0];
			movLocation[3] = end[1];

         movLocation[4] = '\0';
            MOVE* m = createMove(piecePointer,movLocation);
            
            /*if that move could be a capture, in check is true*/
            if(captureMove(piecePointer,m,b,p2) == true){
               deleteMove(m);
               return piecePointer;
               }
            else{
				start[0]='\0';
				end[0]='\0';
				movLocation[0] = '\0';
            deleteMove(m);
            }
         }
   }  
}
  
   return NULL;
}

/*Gets the board array x variable based on location char* c from move*/
char getCharX(int endLocation)
{  int x = endLocation;
   char xChar; 
	if (x == 0)
		xChar = (char)'A';
	else if (x == 1)
		xChar = (char)'B';
	else if (x == 2)
		xChar = (char)'C';
	else if (x == 3)
		xChar = (char)'D';
	else if (x == 4)
		xChar = (char)'E';
	else if (x == 5)
		xChar = (char)'F';
	else if (x == 6)
		xChar = (char)'G';
	else if (x == 7)
		xChar = (char)'H';
   return xChar;
}

/* Gets the board array y variable based on location char* c from move*/
char getCharY(int endLocation)
{
   int y = endLocation;
   char yChar; 
	if (y == 0)
		yChar = (char)'8';
	else if (y == 1)
		yChar = (char)'7';
	else if (y == 2)
		yChar = (char)'6';
	else if (y == 3)
		yChar = (char)'5';
	else if (y == 4)
		yChar = (char)'4';
	else if (y == 5)
		yChar = (char)'3';
	else if (y == 6)
		yChar = (char)'2';
	else if (y == 7)
		yChar = (char)'1';

   return yChar;
}

//return PIECE* that is the king of player passed through
PIECE* getLocationOfPieceKing(BOARD* b, PLAYER* p){
      for(int x = 0;x<8;x++){
         for(int y = 0; y<8;y++){
            if(b->gameBoard[x][y] != NULL){
               if(((b->gameBoard[x][y]->type) == KING)&&(b->gameBoard[x][y]->color == getPlayerColor(p))){
                  return b->gameBoard[x][y];
               }
            }
         }
      }
   return NULL; 
}

bool checkForCheckmate(BOARD* b,PLAYER* p1,PLAYER* p2){
    // List of legal moves for a current piece or the checking piece as a string
    char legalMoves[55] = "";
	char checkLegalMoves[55] = "";

    // Holds piece pointer to the piece putting p1's king in check
    PIECE *checkPiece = checkForCheck(b, p1, p2);

    // Get list of possible moves of piece putting p1's king in check
    MOVE *m;
	char moveString[5] = "";

	// Temp variables
	char end[3] = "";
	char start[3] = "";
	char checkMov[5] = "";

	PIECE *pe;
	MOVE *m2;

	// Conditions for checkmate
	bool canMove = false;
	bool canBlock = false;

	// Obtain string of legal end locations by the checking piece
    for (char x = 'A'; x <= 'H'; x++){
        for (char y = '1'; y <= '8'; y++){
            // moveString is a potential move

            moveString[0] = getLocation(checkPiece)[0];
			moveString[1] = getLocation(checkPiece)[1];
			moveString[2] = x;
			moveString[3] = y;
            m = createMove(checkPiece, moveString);

			end[0] = getEnd(m)[0];
			end[1] = getEnd(m)[1];
			start[0] = getStart(m)[0];
			start[1] = getStart(m)[1];

			pe = getPieceOnBoard(b, end);
		
			checkMov[0] = end[0];
			checkMov[1] = end[1];
			checkMov[2] = start[0];
			checkMov[3] = start[1];

			m2 = createMove(checkPiece, checkMov); // reverse move

			if ( (pe != NULL) && (getColor(pe) == getPlayerColor(p2)) ){ // Do not look at end locations with checking player's own pieces
				moveString[0] = '\0';
				end[0] = '\0';
				start[0] = '\0';
				checkMov[0] = '\0';
				deleteMove(m2);
				deleteMove(m);
				continue;
			}

			if ( (pe == NULL) && (validMove(checkPiece, m, b, p2)) ){ // Empty end location check
				changePieceLocation(m, checkPiece);
				updateBoard(m, b);
				if (checkForCheck(b, p1, p2) != NULL){
					strncat(checkLegalMoves, &x, 1);
					strncat(checkLegalMoves, &y, 1);
				}
				changePieceLocation(m2, checkPiece);
				updateBoard(m2, b);
			}

            if ( (pe != NULL) && (captureMove(checkPiece, m, b, p2)) ){ // End location with piece check
				changePieceLocation(m, checkPiece);
				updateBoard(m, b);
				if (checkForCheck(b, p1, p2) != NULL){
                	strncat(checkLegalMoves, &x, 1);
                	strncat(checkLegalMoves, &y, 1);
				}
				withdrawMove(checkPiece, b, m, pe);
            }

            moveString[0] = '\0';
			end[0] = '\0';
			start[0] = '\0';
			checkMov[0] = '\0';
			deleteMove(m2);
            deleteMove(m);
        }
    }
	/* Determine if any of p1's piece can block a checkmate */
	moveString[0] = '\0';
	start[0] = '\0';
	end[0] = '\0';
	checkMov[0] = '\0';
	PIECE *p;
    char current[3] = "";
    char* compare;
	char location[3] = "";
	MOVE *m3;
	MOVE *m1;

	char currMov[5] = "";

	for(char c = 'A'; c <= 'H'; c++){ 
		for (char r = '1'; r <= '8' ; r++){
            // Generate location of a current piece at 'cr' based on the for loop indexes
			location[0] = c;
			location[1] = r;
            p = getPieceOnBoard(b, location);

            if (p == NULL){ // if there is no piece in that spot, ignore it
				location[0] = '\0';
                continue;
            }

            if (getColor(p) == getPlayerColor(p2)){ // If piece at the spot is an enemy piece, ignore it
				location[0] = '\0';
                continue;
			}

			/* Obtains the attacked player's legal moves (other pieces) */
			for (char a = 'A'; a <= 'H'; a++){
                for (char z = '1'; z <= '8' ; z++){
					moveString[0] = getLocation(p)[0];
					moveString[1] = getLocation(p)[1];
					moveString[2] = a;
					moveString[3] = z;

					m1 = createMove(p, moveString);

					start[0] = getStart(m1)[0];
					start[1] = getStart(m1)[1];
					end[0] = getEnd(m1)[0];
					end[1] = getEnd(m1)[1];

					pe = getPieceOnBoard(b, end);
					
					currMov[0] = end[0];
					currMov[1] = end[1];
					currMov[2] = start[0];
					currMov[3] = start[1];

					m3 = createMove(p, currMov);

					if ( (pe != NULL) && (getColor(pe) == getPlayerColor(p1)) ){
						moveString[0] = '\0';
						end[0] = '\0';
						start[0] = '\0';
						currMov[0] = '\0';
						deleteMove(m3);
						deleteMove(m1);
						continue;
					}

                    if ( (pe == NULL) && (validMove(p, m1, b, p1)) ) {
						changePieceLocation(m1, p);
						updateBoard(m1, b);
						if (checkForCheck(b, p1, p2) == NULL){
							strncat(legalMoves, &a, 1);
							strncat(legalMoves, &z, 1);
						}
						changePieceLocation(m3, p);
						updateBoard(m3, b);
					}

		    if( (pe != NULL) && (captureMove(p, m1, b, p1))){
				changePieceLocation(m1, p);
				updateBoard(m1, b);
				if (checkForCheck(b, p1, p2) == NULL){
					strncat(legalMoves, &a, 1);
					strncat(legalMoves, &z, 1);
				}
				withdrawMove(p, b, m1, pe);
		    }

					moveString[0] = '\0';
					end[0] = '\0';
					start[0] = '\0';
					currMov[0] = '\0';
					deleteMove(m3);
					deleteMove(m1);
                }
            }

			if (legalMoves[0] == '\0')
				continue;
            // Iterate through legalMoves and checkLegalMoves to see if any moves match, return false if any match (can be blocked)
			for (int i = 0; i < strlen(legalMoves) - 1; i += 2){
                // Grabs a substring from legalMoves (e.g. "A2")
                current[0] = legalMoves[i];
				current[1] = legalMoves[i+1];
				compare = strstr(checkLegalMoves, current);
                if (compare != NULL){
                    canBlock = true; // Means that any piece can block the checking piece from capturing the king
                }
               	current[0] = '\0';
            }

			location[0] = '\0';
            legalMoves[0] = '\0'; // clear string of legal moves to check the next spaces
        }
    }	

	/* Determine if the king is unable to move out of check */
  
	if(checkForCheck(b,p1,p2) != NULL){
       if(kingCheckMove(checkLegalMoves, b, p1, p2) == true){
		canMove = true; // Means that the king can move out of check
      }
   }

	if (canMove || canBlock){ // If the king can move out of check or the attacked player can block a checkmate, it is not checkmate -> return false
		return false;
	}

	// p1 can neither block or move out of check, it is a checkmate -> return true
	printf("\nPlayer %d is in checkmate. Player %d wins!\n", p1->num, p2->num);
	return true;
}

/* Returns true if king is able to moev out of check */
bool kingCheckMove(char* checkLegalMoves, BOARD *b,PLAYER* p1,PLAYER* p2)
{
	PIECE *kingPiece = getLocationOfPieceKing(b, p1);
	PIECE *pe;
	MOVE *m;
	MOVE *m2;
	char kingMov[5] = "";
	char legalMoves[55] = "";
	char moveString[5] = "";
	char end[3] = "";
	char start[3] = "";
	/* Gets the legal moves for the king */
	for (char a = 'A'; a <= 'H'; a++){
		for (char z = '1'; z <= '8'; z++){
			moveString[0] = getLocation(kingPiece)[0];
			moveString[1] = getLocation(kingPiece)[1];
			moveString[2] = a;
			moveString[3] = z;
			m = createMove(kingPiece, moveString);

			end[0] = getEnd(m)[0];
			end[1] = getEnd(m)[1];
			start[0] = getStart(m)[0];
			start[1] = getStart(m)[1];

			pe = getPieceOnBoard(b, end);

			kingMov[0] = end[0];
			kingMov[1] = end[1];
			kingMov[2] = start[0];
			kingMov[3] = start[1];
			m2 = createMove(kingPiece, kingMov);
		
			if ( (pe != NULL) && (getColor(pe) == getPlayerColor(p1))) { // end piece is the attacked player's own piece
				moveString[0] = '\0';
				end[0] = '\0';
				start[0] = '\0';	
				kingMov[0] = '\0';
				deleteMove(m);
				deleteMove(m2);
				continue;
			}

			if ( (pe == NULL) && (validMove(kingPiece, m, b, p1)) ){ // Move to a null end location
				changePieceLocation(m, kingPiece);
				updateBoard(m, b);	
				if (checkForCheck(b, p1, p2) == NULL){ // see if king cannot move out of check
					strncat(legalMoves, &a, 1);
					strncat(legalMoves, &z, 1);
				}
				changePieceLocation(m2, kingPiece);
				updateBoard(m2, b);
			}

			if ( (pe != NULL) && (captureMove(kingPiece, m, b, p1)) ){ // move to an end location with an enemy piece
				changePieceLocation(m, kingPiece);
				updateBoard(m, b);
				if (checkForCheck(b, p1, p2) == NULL){ // see if king can move out of check
					strncat(legalMoves, &a, 1);
					strncat(legalMoves, &z, 1);
				}
				withdrawMove(kingPiece, b, m, pe);
			}
			moveString[0] = '\0';
			end[0] = '\0';
			start[0] = '\0';	
			kingMov[0] = '\0';
			deleteMove(m);
			deleteMove(m2);
		}
	}

	if (legalMoves[0] == '\0') // King has no legal moves that gets it out of check
		return false;
	else
		return true;        
}

//returns true if the player can make a move that'll put the other player in check
bool LCheck(BOARD *b, PLAYER* p1, PLAYER* p2)
{

    // List of legal moves for a current piece or the checking piece as a string
    char legalMoves[55] = "";

    // Get list of possible moves of piece putting p1's king in check
	char moveString[5] = "";

	// Temp variables
	char end[3] = "";
	char start[3] = "";

	PIECE *p;
	char location[3] = "";
	MOVE *m3;
	MOVE *m1;


	PIECE *pe;

	char currMov[5] = "";

       for(char c = 'A'; c <= 'H'; c++){ 
	for (char r = '1'; r <= '8' ; r++){
            // Generate location of a current piece at 'cr' based on the for loop indexes
			location[0] = c;
			location[1] = r;
            p = getPieceOnBoard(b, location);

            if (p == NULL){ // if there is no piece in that spot, ignore it
				location[0] = '\0';
                continue;
            }

            if (getColor(p) == getPlayerColor(p2)){ // If piece at the spot is an enemy piece, ignore it
				location[0] = '\0';
                continue;
			}

			/* Obtains the attacked player's legal moves (other pieces) */
			for (char a = 'A'; a <= 'H'; a++){
                for (char z = '1'; z <= '8' ; z++){
					moveString[0] = getLocation(p)[0];
					moveString[1] = getLocation(p)[1];
					moveString[2] = a;
					moveString[3] = z;

					m1 = createMove(p, moveString);

					start[0] = getStart(m1)[0];
					start[1] = getStart(m1)[1];
					end[0] = getEnd(m1)[0];
					end[1] = getEnd(m1)[1];

					pe = getPieceOnBoard(b, end);
					
					currMov[0] = end[0];
					currMov[1] = end[1];
					currMov[2] = start[0];
					currMov[3] = start[1];

					m3 = createMove(p, currMov);

					if ( (pe != NULL) && (getColor(pe) == getPlayerColor(p1)) ){
						moveString[0] = '\0';
						end[0] = '\0';
						start[0] = '\0';
						currMov[0] = '\0';
                  if(m3 != NULL){
						   deleteMove(m3);
                  }
                  if(m1 != NULL){
						   deleteMove(m1);
                  }
						continue;
					}

                    if ( (pe == NULL) && (validMove(p, m1, b, p1)) ) {
						changePieceLocation(m1, p);
						updateBoard(m1, b);
						if (checkForCheck(b, p1, p2) != NULL){
							strncat(legalMoves, &a, 1);
							strncat(legalMoves, &z, 1);
						}
						withdrawMove(p, b, m1, pe);
	   	    		}

		    		if( (pe != NULL) && (captureMove(p, m1, b, p1))){

						changePieceLocation(m1, p);
						updateBoard(m1, b);
						if (checkForCheck(b, p1, p2) != NULL){
							strncat(legalMoves, &a, 1);
							strncat(legalMoves, &z, 1);
						}
						withdrawMove(p, b, m1, pe);
		    }

			moveString[0] = '\0';
			end[0] = '\0';
			start[0] = '\0';
			currMov[0] = '\0';
			if(m3 != NULL){
            deleteMove(m3);
            }
         if(m1 != NULL){
			   deleteMove(m1);
            }
                }
            }

		if (legalMoves[0] != '\0') // A legal move has been found so p1 will not be in check
				return true;

			location[0] = '\0';
            legalMoves[0] = '\0'; // clear string of legal moves to check the next spaces
        }
    }	
	return false;
}

//checking for stalemate, where p1 is the player being checked if they are in a stalemate
bool checkForSM(BOARD *b, PLAYER* p1, PLAYER* p2)
{

    // List of legal moves for a current piece or the checking piece as a string
    char legalMoves[55] = "";

    // Get list of possible moves of piece putting p1's king in check
	char moveString[5] = "";

	// Temp variables
	char end[3] = "";
	char start[3] = "";

	PIECE *p;
	char current[3] = "";
	char location[3] = "";
	MOVE *m3;
	MOVE *m1;


	PIECE *pe;

	char currMov[5] = "";

	bool LegalMoves = false;

       for(char c = 'A'; c <= 'H'; c++){ 
	for (char r = '1'; r <= '8' ; r++){
            // Generate location of a current piece at 'cr' based on the for loop indexes
			location[0] = c;
			location[1] = r;
            p = getPieceOnBoard(b, location);

            if (p == NULL){ // if there is no piece in that spot, ignore it
				location[0] = '\0';
                continue;
            }

            if (getColor(p) == getPlayerColor(p2)){ // If piece at the spot is an enemy piece, ignore it
				location[0] = '\0';
                continue;
			}

			/* Only looks at pieces that are p1's, obtains the legal moves for that */
			for (char a = 'A'; a <= 'H'; a++){
                for (char z = '1'; z <= '8' ; z++){
					moveString[0] = getLocation(p)[0];
					moveString[1] = getLocation(p)[1];
					moveString[2] = a;
					moveString[3] = z;

					m1 = createMove(p, moveString);

					start[0] = getStart(m1)[0];
					start[1] = getStart(m1)[1];
					end[0] = getEnd(m1)[0];
					end[1] = getEnd(m1)[1];

					pe = getPieceOnBoard(b, end);
					
					currMov[0] = end[0];
					currMov[1] = end[1];
					currMov[2] = start[0];
					currMov[3] = start[1];

					m3 = createMove(p, currMov);

					if ( (pe != NULL) && (getColor(pe) == getPlayerColor(p1)) ){ // Cannot capture own piece
						moveString[0] = '\0';
						end[0] = '\0';
						start[0] = '\0';
						currMov[0] = '\0';
						deleteMove(m3);
						deleteMove(m1);
						continue;
					}

                    if ( (pe == NULL) && (validMove(p, m1, b, p1)) ) { // Valid move to an empty end location
						changePieceLocation(m1, p);
						updateBoard(m1, b);
						if (checkForCheck(b, p1, p2) == NULL){
						  if(!LCheck(b,p1, p2)){ // Looks a move ahead to see if player 1 will still be in check after this move
							strncat(legalMoves, &a, 1);
							strncat(legalMoves, &z, 1);
						  }
						}
						withdrawMove(p, b, m1, pe);
	   	    		}

		    		if( (pe != NULL) && (captureMove(p, m1, b, p1))){ // Move to a filled end location (capture)
						changePieceLocation(m1, p);
						updateBoard(m1, b);
						if (checkForCheck(b, p1, p2) == NULL){// Looks a move ahead to see if player 1 will still be in check after this move
						  if(!LCheck(b,p1, p2)){
							strncat(legalMoves, &a, 1);
							strncat(legalMoves, &z, 1);
						  }
					}
						withdrawMove(p, b, m1, pe);
		    }

			moveString[0] = '\0';
			end[0] = '\0';
			start[0] = '\0';
			currMov[0] = '\0';
			deleteMove(m3);
			deleteMove(m1);
            }
		}

			if (legalMoves[0] != '\0') // At least one legal move has been found so p1 won't be in check in the future
				LegalMoves = true;

            current[0] = '\0';
			location[0] = '\0';
            legalMoves[0] = '\0'; // clear string of legal moves to check the next spaces
        }
    }
	char* kingLoc;
   kingLoc = getLocationOfKing(b,p1);
	//if the king is NOT in checkmate && no legal moves for king bc it'll put it in check
	if((!checkForCheck(b, p1,p2)) && (!kingCheckMove(kingLoc,b,p1,p2)) && !LegalMoves){
      free(kingLoc);
		return true;	
   }
	else{
      free(kingLoc);
		return false;
      }
	
}

//checking for insufficient material
bool checkForIM(BOARD *b, PLAYER *p1, PLAYER *p2)
{
	//counts the # of pieces on the board
	int qwcount=0, qbcount=0, rwcount=0, rbcount=0, bwcount=0, bbcount=0, nwcount=0, nbcount=0, kwcount=0, kbcount=0, pwcount=0, pbcount=0;
	//checks if it is insufficient material
	for(int x = 0; x <8; x++){
	    for(int y = 0; y < 8; y++){
		if(rPieceOnBoard(b,x,y))
		{
			PIECE *p = rPieceOnBoard(b,x,y);
			// Switch for piece type movement checking 
			switch (getType(p)){
				case PAWN:
					if(getColor(p)=='w')
						pwcount++;
					else if(getColor(p) == 'b')
						pbcount++;
					break;	
				case ROOK:
					if(getColor(p)=='w')
						rwcount++;
					else if(getColor(p) == 'b')
						rbcount++;
					break;
				case KNIGHT:
					if(getColor(p)=='w')
						nwcount++;
					else if(getColor(p) == 'b')
						nbcount++;
					break;
				case BISHOP:
					if(getColor(p)=='w')
						bwcount++;
					else if(getColor(p) == 'b')
						bbcount++;
					break;
				case KING:
					if(getColor(p)=='w')
						kwcount++;
					else if(getColor(p) == 'b')
						kbcount++;
					break;
				case QUEEN:
					if(getColor(p)=='w')
						qwcount++;
					else if(getColor(p) == 'b')
						qbcount++;
					break;
				default:
					break;
			}//switch

		}//ifpiece
	    }//for
	}//for

	//IM for king v king
	if((kbcount==kwcount) && ((pwcount==0)&&(pbcount==0)) && ((qwcount==0)&&(qbcount==0)) && ((rwcount == 0)&&(rbcount==0)) 
	    && ((bbcount==0)&&(bwcount==0)) && ((nwcount==0)&&(nbcount==0)))
		return true;

	//IM for king & bishop v king
	else if((kbcount==kwcount) && ((pwcount==0)&&(pbcount==0)) && ((qwcount==0)&&(qbcount==0)) && ((rwcount == 0)&&(rbcount==0)) 
	    && (((bbcount==1)&&(bwcount==0)) || ((bbcount==0)&&(bwcount==1))) && ((nwcount==0)&&(nbcount==0)))
		return true;

	//IM for king & knight v king
	else if((kbcount==kwcount) && ((pwcount==0)&&(pbcount==0)) && ((qwcount==0)&&(qbcount==0)) && ((rwcount == 0)&&(rbcount==0)) 
	    && ((bbcount==0)&&(bwcount==0)) && (((nwcount==0)&&(nbcount==0))||((nwcount==0)&&(nbcount==1))))
		return true;

	//IM for king bishop v king bishop
	else if((kbcount==kwcount) && ((pwcount==0)&&(pbcount==0)) && ((qwcount==0)&&(qbcount==0)) && ((rwcount == 0)&&(rbcount==0)) 
	         && ((bbcount==1)&&(bwcount==1)) && ((nwcount==0)&&(nbcount==0)))
		return true;
	//if not then return false and it'll continue the game
	else 
		return false;
}

/*Gets current location of the king of the player passed through*/
char* getLocationOfKing(BOARD* b, PLAYER* p){
	char* kingLocation = NULL;
   char king[3] = "";

   kingLocation = king;
      for(int x = 0; x < 8; x++){
         for(int y = 0; y < 8; y++){
            if(b->gameBoard[x][y] != NULL){
               if(((getType(b->gameBoard[x][y])) == KING) && (getColor(b->gameBoard[x][y]) == getPlayerColor(p))){
					king[0] = (b->gameBoard[x][y]->location)[0];
               king[1] = (b->gameBoard[x][y]->location)[1];
                  break;
               }
            }
         }
      } 
   
   return strdup(kingLocation);
}


/*Gets location of an enemy piece based on x y cordinates*/
PIECE* getLocationOfEnemyPiece(BOARD* b, PLAYER* p,int x, int y)
{
   PIECE* piece;
	if(((b->gameBoard[x][y])==NULL)||(b->gameBoard[x][y]->color != getPlayerColor(p))){
      piece = NULL;
   }
   else{
	   if (((b->gameBoard[x][y]) != NULL)&&((b->gameBoard[x][y]->color) == getPlayerColor(p)))
		   piece =  b->gameBoard[x][y];
   }
   return piece;
}
