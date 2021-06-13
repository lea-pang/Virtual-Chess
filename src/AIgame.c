/*Authors:
 * Stella Zhou
 * Lea Pang
 *
 * Updates:
 * 05/11/21 SZ added tie conditions + enpassant + castling conditions
 * 05/06/21 SZ fixed random AI to be time efficient
 * 05/02/21 SZ fixed edge cases for random AI (when colors are switched)
 * 04/29/21 SZ added more functions for AI functionality (more framework)
 * 04/28/21 SZ Started randomAI + finished?, found bugs, did not solve
 * 04/27/21 SZ Started AI framework (update boards etc.)
 * 04/20/21 SZ Started Human_v_AI + confirmMove 
 * 05/02/21 LP Fixed begAI() to not segfault, changed some string assignments, Human_v_AI() reformatting
 * 05/06/21 LP Input validation mostly working (not working when there are spaces), fixed withdrawMove() and promotionMove()
 * 05/07/21 LP AI promotion function, maybe fixed withdraw and input validation when an invalid move is made (have not tested)
 */

//header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "Piece.h"
#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "moveList.h"
#include "Win.h"
#include "AIgame.h"
#include "humanGame.h"

//simple AI function
void Human_v_AI(PLAYER *p1, PLAYER *p2, BOARD *b)
{
	printf("\nHuman v. AI game has started.\n");

    //variable initialization
    char input[5] = "";
	char* user_input = NULL;
	char end[3] = "";
	char start[3] = "";
    bool c = false;
 
    //booleans for check + move conditions   
    bool win = false;
	bool exit = false;
 
    //counts the number of turns for the 50 move rule
    int count = 0;
    int Pmove = -1;
    int AImove = -1;
	
    int AIchoice = 0;
	char AIchoice_input[30] = "";

	MOVE *m;
    AIMenu();

	while (true){
		scanf("%s", AIchoice_input);
		if (strcmp(AIchoice_input, "1") == 0){
			AIchoice = 1;
			break;
		}
		else if(strcmp(AIchoice_input, "2") == 0){
			AIchoice = 2;
			break;
		}
		else{
			printf("\nPlease input a valid choice: ");
			AIchoice_input[0] = '\0';
		}
	}

    //prints inital board
    printBoard(b);	

    //game will keep going while there is no win or tie
    while(!win){

	//if human is white they go first
	if(getPlayerColor(p1) == 'w')
	{

		//check at the beginning of a turn if the current player is in check
        	if((checkForCheck(b,p1,p2) != NULL)){
         	      printf("Player 1 is in check. You must move the king out of check, or block to get out of check.\nIf no valid move, Player 1 is in checkmate\n");
        	}
       
	    while(!(Pmove==1||Pmove==2)){
			printf("Input your move: "); // Move input and validation
			user_input = moveInput(b);
			input[0] = user_input[0];
			input[1] = user_input[1];
			input[2] = user_input[2];
			input[3] = user_input[3];
	      free(user_input);

			if (strcmp(input, "exit") == 0){ // User inputs "exit"
				exit = true;
				break;
			}

			c = confirmMove(); // Asks user to confirm move

			if(c == false){ 
				printf("\nYou decided to withdraw your move.\n\n");
				continue;
		     }	

  	     	/* Parses user input */
			start[0] = input[0];
			start[1] = input[1];

		/* Initializes temporary PIECE *p to piece at the start location */
		PIECE *p = getPieceOnBoard(b, start);

		/* Creates new move based on user input */
		m = createMove(p, input);

		/* Gets end piece*/
		end[0] = getEnd(m)[0];
		end[1] = getEnd(m)[1];
		PIECE *pe = getPieceOnBoard(b, end);
		
		/*  Updates the board */
		Pmove = playerMove(p,m,b,p1,p2,pe);
		
		if(Pmove == 1)	//counts the # of turns if the player move was capture or a pawn move it'll add move
			count++;
		else if(Pmove == 2)
			count = 0;
	    }
	}//end of p1

        if(checkForCheck(b,p2,p1) != NULL){
            if(checkForCheckmate(b,p2,p1) == true)
				win = true;
	}
/*Win + tie conditions*/
	if(win) // Checkmate is detected
		break;
	if (exit) // User wants to exit
		break;

	//stalemate
	if(checkForSM(b,p1,p2))
	{
		printf("\nThis game has ended with a stalemate. \n");
		break;
	}
	//checking for insufficient material
	else if(checkForIM(b,p2,p1))
	{
		printf("\nThis game has ended in a draw due to insufficient material. \n");
		break;
	}	

	//if greater than the 50 move rule, it will ask the user if they want to tie
	if(count > 100)
	{
		bool claim = false;
		claim = claimTie();
		if(claim)
			break;
		else
			continue;
		
	}
/*end of win + tie condition check */

	/* Switches between AI functions based on initial menu AI input */
	switch(AIchoice){
		case 1:
			AImove = begAI(b,p2,p1);
			break;
		case 2:
			AImove = randAI(b,p2,p1);
			break;
		default:
			break;
	}
	
	/* 50 move rule counter for AI*/
	if(AImove == 1)	//counts the # of turns if the player move was capture or a pawn move it'll add move
		count++;
	else if(AImove == 2)
		count = 0;
		
	/*Win + tie condition check*/
	/*check for WIN*/
        if(checkForCheck(b,p2,p1) != NULL){
            if(checkForCheckmate(b,p2,p1) == true)
			win = true;
	}
	if(win)
		break;
	
	//stalemate
	if(checkForSM(b,p1,p2))
	{
		printf("\nThis game has ended with a stalemate. \n");
		break;
	}
	//checking for insufficient material
	else if(checkForIM(b,p2,p1))
	{
		printf("\nThis game has ended in a draw due to insufficient material. \n");
		break;
	}	

	//if greater than the 50 move rule, it will ask the user if they want to tie
	if(count > 100)
	{
		bool claim = false;
		claim = claimTie();
		if(claim)
			break;
		else
			continue;
		
	}
	/*Win + tie condition check end */

	/* If human is black they go second */
	if(getPlayerColor(p1) == 'b')
	{
	
		//check at the beginning of a turn if the current player is in check
        	if((checkForCheck(b,p1,p2) != NULL)){
         	      printf("Player 1 is in check. You must move the king out of check, or block to get out of check.\nIf no valid move, Player 1 is in checkmate\n");
        	}

		while (!(Pmove==1||Pmove==2)){
			printf("Input your move: "); // Move input and validation
			user_input = moveInput(b);
			input[0] = user_input[0];
			input[1] = user_input[1];
			input[2] = user_input[2];
			input[3] = user_input[3];
         free(user_input);
			if (strcmp(input, "exit") == 0){ // User inputs "exit"
				exit = true;
				break;
			}

			c = confirmMove(); // Asks user to confirm move

		if(c == false){
				printf("\nYou decided to withdraw your move.\n\n");
				continue;
			}
	
  	    	/* Parses user input */
    		start[0] = input[0];
			start[1] = input[1];
		
		/* Initializes temporary PIECE *p to piece at the start location */
		PIECE *p = getPieceOnBoard(b, start);

		/* Creates new move based on user input */
		m = createMove(p, input);

		/* Gets end piece */
		end[0] = getEnd(m)[0];
		end[1] = getEnd(m)[1];
		PIECE *pe = getPieceOnBoard(b, end);
		
		/*  Updates the board */
		Pmove = playerMove(p,m,b,p1,p2,pe);
		
		if(Pmove == 1)	//counts the # of turns if the player move was capture or a pawn move it'll add move
			count++;
		else if(Pmove == 2)
			count = 0;	
	    }
	}
	/*Win + tie condition check */

	/* check for WIN */	
        if(checkForCheck(b,p1,p2) != NULL){
            if(checkForCheckmate(b,p1,p2) == true)
			win = true;
	}
	if(win) // Checkmate is detected
		break;
	if (exit) // User wants to exit
		break;

	//stalemate
	if(checkForSM(b,p1,p2))
	{
		printf("\nThis game has ended with a stalemate. \n");
		break;
	}
	//checking for insufficient material
	else if(checkForIM(b,p2,p1))
	{
		printf("\nThis game has ended in a draw due to insufficient material. \n");
		break;
	}	

	//if greater than the 50 move rule, it will ask the user if they want to tie
	if(count > 100)
	{
		bool claim = false;
		claim = claimTie();
		if(claim)
			break;
		else
			continue;
		
	}
	/*End of Win + tie condition check */

	c = false;
	Pmove = -1;

    }//while if not in checkmate

   printToFile(p1,p2);
   printf("\nThe game has ended.\n\n");
   removeList();
}

//prints the AI Menu to select which level of difficult user would like to play
void AIMenu()
{
	printf("\nAI Difficulty Levels\n---------------------\n");
	printf("1. Easy \n"); //first spot available AI 
	printf("2. Medium\n"); //random AI
	printf("Please make your choice: ");
}

//checks to see if the player want to go with this move
bool confirmMove()
{
	char choice[30] = "";

	printf("Are you sure you want to move here? Input y for yes and n for no: ");

	while (1){
		scanf("%s", choice);

		//if the inputs are not y or n, the game will ask him to input something valid
		if (strcmp(choice, "y") == 0)
			return true;

		else if (strcmp(choice, "n") == 0)
			return false;

		else{
			printf("\nPlease input a valid choice(y or n): ");
		}
	}
	return false;
}

//function that updates everything based on the user's input
int playerMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *p1, PLAYER *p2, PIECE *pe)
{
	bool c = false;
	bool capture = false;
	bool valid = false;

	/*Checks if the end location is filled, it will check if the end location piece is an enemy*/
	if(!emptySpace(m,b))
		c = enemySpace(p1, pe, m, b);

	/*if it is an enemy piece, it will check for capture, or else it will think 
	 *its the same player piece and check if its a valid move*/
	if (c){
		capture = captureMove(p,m,b,p1);
	        //updates the board to see if move puts player into check
         	changePieceLocation(m,p);
         	updateBoard(m,b);

         	//if with new updated board, player is still in check
         	//capture is changed to false and board is reset
         	if((checkForCheck(b,p1,p2) != NULL)&&(capture == true)){
         	     capture = false;
         	     withdrawMove(p,b,m, pe);
         	}
		  
	        //if not in check, board still resets and move is made as normal
        	else{
        		 withdrawMove(p,b,m, pe);
            	}
         }
			
	else{
		 valid = validMove(p,m,b,p1);
        	 //Checks if a valid move puts the king in check
             	 changePieceLocation(m,p);
            	 updateBoard(m,b);
            
		 //if in check but also a valid move after board change 
        	 //valid is false and board is reset 
        	 if((checkForCheck(b,p1,p2) != NULL)&&(valid == true)){
        		 valid = false;
      		         withdrawMove(p,b,m, pe);
            	 }
            
		//if a regular move, board is reset, then regular move takes place
        	else{
        		withdrawMove(p,b,m, pe);
        	}
	}
	/* Check for possible promotion */
	promotionMove(p, m, b, p1);

	/* if the capture is there, it will update the board, and the piece will be at the end location (skipping valid location)*/
	if(capture == true){
		//changes location of piece bc there's a capture
		printf("A piece has been captured! \n");
		changePieceLocation(m, p); /* changes the location of current piece to go to the end location (and captures) */
		updateBoard(m,b); //updates the board
		printBoard(b);

		//if the move is valid, the piece move counter is added
		addMove(p);

		addNode(m);
      deletePiece(pe); 
		return 1;
	}

	/*needs capture is NOT true to go check if valid move + updating the board*/
	else if((valid == true) && (capture != true)){
	
		if(enPassant(p,m,b))
			enPassantCapture(p,m,b);
	
		if (castlingCondition(p, m, b)) //castling
			castling(m, b);

		changePieceLocation(m, p); /* Changes current location attribute of piece*/
		updateBoard(m, b); /* Updates the current board using the move */
		printBoard(b);

		//if the move is valid, the piece move counter is added
		addMove(p);
		
		addNode(m);

		return 2;
	}	
	else{

		//if valid and capture are both false check if in checkmate
        	//if not in checkmate player has to make a valid move to get out of check
            	if(checkForCheck(b,p1,p2) != NULL){
               	    if(checkForCheckmate(b,p1,p2) == true)
			return 1;
		}

		printf("\nInvalid move.\n");
		return -1;
	}
	
}

/*Randomizes the Move input + piece */
int randAI(BOARD *b, PLAYER *p2, PLAYER *p1)
{
	//tells the user that the AI is finding a move bc random takes awhile
	printf("AI is finding a move...\n");
	srand(time(NULL));

	//all the variables needed to start finding the move
	//emptyspace before capture variable
	bool c = false;
	//variables for capture + valid
	bool capture = false;
	bool valid = false;
	//input string
	char input[5] = "";

	bool player = true;

	/*Algorithm specific for when AI is the black side (random move spawn) */
	while(getPlayerColor(p2) == 'b' && player == true){

	    /*randomly finds the array spot which would be the START location*/
	    int x = rand()%8;
	    int y = rand()%8;
		
	    //end location char
	    char end[3] = "";
	 
	    /* Initializes temporary PIECE *p to piece at the start RANDOM location that the AI chose */
	    if(rPieceOnBoard(b,x,y)){
	    	PIECE *p = rPieceOnBoard(b, x, y);
	    	strncpy(input, getLocation(p),2);
	
	    	//randomizes the end spot
	    	char w = (char)('A' + rand()%6);
	    	char z = (char)('7' - rand()%6);

	    	end[0] = w;
	    	end[1] = z;    	    
	    
	    	input[2] = w;
	    	input[3] = z;

	    	/* Creates new move based on user input */
	    	MOVE *m = createMove(p, input);

	    	/*get try to see if there's a piece at the end of the random location*/
	    	PIECE *pe = getPieceOnBoard(b, end);
	    	    
	    	/*Checks if the end location is filled, it will check if the end location piece is an enemy*/
	    	if(!emptySpace(m,b))
			c = enemySpace(p2, pe, m, b);

		/*if it is an enemy piece, it will check for capture, or else it will think 
		 *its the same player piece and check if its a valid move*/
		if (c && !emptySpace(m,b)){
			capture = captureMove(p,m,b,p2);
		}
		else{
		 	valid = validMove(p,m,b,p2);
         	}
  	    	
		/* if the capture is there, it will update the board, and the piece will be at the end location (skipping valid location)*/
	    	if(capture){
		
			//updates the board to see if move puts player into check
			changePieceLocation(m,p);
			updateBoard(m,b);

			//if with new updated board, player is still in check
			//capture is changed to false and board is reset
        		if((checkForCheck(b,p2,p1) != NULL)){
        			capture = false;
        			withdrawMove(p,b,m, pe);
        		}

			else{ 
				promotionAI(p,m,b,p2);
				printBoard(b);

				printf("\nThe AI has found a move!\n\n");

				//if the move is valid, add move counter to the piece
				addMove(p);

				addNode(m);	
            deletePiece(pe);
				player = false;
				return 1;
			}
        	
	    	}

	    	/*needs capture is NOT true to go check if valid move + updating the board*/
	    	else if(valid && (!capture)){
	
			if(enPassant(p,m,b))
				enPassantCapture(p,m,b);			

			if (castlingCondition(p, m, b)) //castling
				castling(m, b);
	
	       		//updates the board to see if move puts player into check
         		changePieceLocation(m,p);
         		updateBoard(m,b);

         		//if with new updated board, player is still in check
         		//capture is changed to false and board is reset
         		if((checkForCheck(b,p2,p1) != NULL)){
         	     		capture = false;
         	     		withdrawMove(p,b,m, pe);
			}
         		
		 	else{ 
				promotionAI(p,m,b,p2);
				printBoard(b);

				printf("\nThe AI has found a move!\n\n");
				//if the move is valid, add to piece counter
				addMove(p);

				addNode(m);
				
				player = false;
				return 2;
			}
		}
      if(m != NULL){
         deleteMove(m);
      }	
  	    }
		//reset values
		c = false;
		capture = false;
		valid = false;
	}

	/*Random algorith specifically for when the AI is the white side*/
	while(getPlayerColor(p2) == 'w' && player == true){
	    /*randomly finds the array spot which would be the start location*/
	    int x = rand()%8;
	    int y = rand()%8;
		
	    //end location char
	    char end[3] = "";
	 
	    /* Initializes temporary PIECE *p to piece at the start RANDOM location that the AI chose */
	    if(rPieceOnBoard(b,x,y)){
	    	PIECE *p = rPieceOnBoard(b, x, y);
	    	strncpy(input, getLocation(p),2);
	
	    	//randomizes the end spot 
	    	char w = (char)('A' + rand()%6);
	    	char z = (char)('2' + rand()%6);

	    	end[0] = w;
	    	end[1] = z;    	    
	    
	    	input[2] = w;
	    	input[3] = z;

		/* Creates new move based on user input */
	    	MOVE *m = createMove(p, input);

	    	/*get try to see if there's a piece at the end of the random location*/
	    	PIECE *pe = getPieceOnBoard(b, end);
	    	    
	    	/*Checks if the end location is filled, it will check if the end location piece is an enemy*/
	    	if(!emptySpace(m,b))
			c = enemySpace(p2, pe, m, b);
	    	/*if it is an enemy piece, it will check for capture, or else it will think 
	    	 *its the same player piece and check if its a valid move*/

		if (c && !emptySpace(m,b)){
			capture = captureMove(p,m,b,p2);
         	}
			
		else{
		 	valid = validMove(p,m,b,p2);
         	}
  	    	
  	    	/* if the capture is there, it will update the board, and the piece will be at the end location (skipping valid location)*/
	    	if(capture){
			
	        	//updates the board to see if move puts player into check
         		changePieceLocation(m,p);
         		updateBoard(m,b);

			//if with new updated board, player is still in check
			//capture is changed to false and board is reset
			if((checkForCheck(b,p2,p1) != NULL)){	
        			capture = false;
        			withdrawMove(p,b,m, pe);
         		}
			else{ 
				promotionAI(p, m, b, p2); // Check promotion
				printBoard(b);

				printf("\nThe AI has found a move!\n\n");

				//if move is valid add to piece's move counter
				addMove(p);

				addNode(m);
            
				player = false;
            deletePiece(pe);
				return 1;
			}
	    	}

	    	/*needs capture is NOT true to go check if valid move + updating the board*/
	    	else if(valid && (!capture)){

			if(enPassant(p,m,b))
				enPassantCapture(p,m,b);
	
			if (castlingCondition(p, m, b)) //castling
				castling(m, b);

	        	//updates the board to see if move puts player into check
         		changePieceLocation(m,p);
         		updateBoard(m,b);

			//if with new updated board, player is still in check
			//capture is changed to false and board is reset
        		if((checkForCheck(b,p2,p1) != NULL)){
        			capture = false;
        			withdrawMove(p,b,m, pe);
         		}
			else{ 
				promotionAI(p, m, b, p2); // Check promotion
				printBoard(b);

				printf("\nThe AI has found a move!\n\n");

				//if move is valid add to piece's move counter
				addMove(p);

				addNode(m);
           
				player = false;
				return 2;
			}
		}	
      if(m != NULL){
         deleteMove(m);
      }
  	    }	
		//reset values
		c = false;
		capture = false;
		valid = false;
	}
	return -1;
}

/* This AI is meant to make a move at the first available spot */
int begAI(BOARD*b, PLAYER *p2, PLAYER *p1)
{
	//beginner level should aim to do the first available move
	int i, j, x, y;

	//string initialization
	char end[3] = "";
	char input[5] = "";

	//emptyspace before capture variable
	bool c = false;
   MOVE* m;
	//variables for capture + valid
	bool capture = false;
	bool valid = false;

	for(i = 0; i < 8; i++){
	    for(j = 0; j < 8; j++){

		if(rPieceOnBoard(b,i,j))
		{
	    /* Initializes temporary PIECE *p to piece at the start RANDOM location that the AI chose */
	    PIECE *p = rPieceOnBoard(b,i,j);
	    strncpy(input, getLocation(p),2);
			
			//another loop to get a spot????
			for(x = 0; x < 8; x++){
			     for(y = 0; y < 8; y++){
				if(rPieceOnBoard(b,x,y)==NULL)
				{
				 	char xe = 'A' + y;
					char ye = '1' + x;

	    				end[0] = xe;
			  	        end[1] = ye;    
	    				
					input[2] = xe;
	    				input[3] = ye;
	    			
					/* Creates new move based on user input */
	    				m = createMove(p, input);
	
	    				/*get try to see if there's a piece at the end of the random location*/
	    				PIECE *pe = getPieceOnBoard(b, end);
	    		    
	    				/*Checks if the end location is filled, it will check if the end location piece is an enemy*/
	    				if(!emptySpace(m,b))
							c = enemySpace(p2, pe, m, b);

	    				/*if it is an enemy piece, it will check for capture, or else it will think 
	     				*its the same player piece and check if its a valid move*/

					if (c && !emptySpace(m,b)){

						capture = captureMove(p,m,b,p2);
         				}		
			
					else{
		 				valid = validMove(p,m,b,p2); 
         				}
		
  	    				/* if the capture is there, it will update the board, and the piece will be at the end location (skipping valid location)*/
	    				if(capture == true){

	        				//updates the board to see if move puts player into check
         					changePieceLocation(m,p);
         					updateBoard(m,b);

         					//if with new updated board, player is still in check
         					//capture is changed to false and board is reset
         					if((checkForCheck(b,p2,p1) != NULL)){
         	     					capture = false;
         	     					withdrawMove(p,b,m, pe);
         					}
		 				else{ 
							promotionAI(p, m, b, p2);
							printBoard(b);

							printf("\nThe AI has found a move!\n\n");

							//if move is valid add to piece's move counter
							addMove(p);

							addNode(m);
                     deletePiece(pe);
							return 1;
					        }
        	
	    				}

	    				/*needs capture is NOT true to go check if valid move + updating the board*/
	    				else if((valid == true) && (capture != true)){
						
						if(enPassant(p,m,b))
							enPassantCapture(p,m,b);
	
						if (castlingCondition(p, m, b)) //castling
							castling(m, b);

	        				//updates the board to see if move puts player into check
         					changePieceLocation(m,p);
         					updateBoard(m,b);

         					//if with new updated board, player is still in check
         					//capture is changed to false and board is reset
         					if((checkForCheck(b,p2,p1) != NULL)){
         	     					valid = false;
         	     					withdrawMove(p,b,m, pe);
         					}
		 				else{ 
							promotionAI(p, m, b, p2);
							printBoard(b);

							printf("\nThe AI has found a move!\n\n");

							//if move is valid add to piece's move counter
							addMove(p);
                     
							addNode(m);
                     
                     return 2;
						}
					}
                  if(m != NULL){
                     deleteMove(m);
                  }
				    }//if

					//reset values
					c = false;
					capture = false;
					valid = false;
				}//2nd for
			}//2nd for
		}//if
	}//1st for
    }//1st for	
    return -1;
} 

/*Gets the piece of the initial random piece based on the row + col randomly produced by AI*/
PIECE* rPieceOnBoard(BOARD *b, int row, int col)
{
	if ((b->gameBoard[row][col]) != NULL)
		return b->gameBoard[row][col];
	else
		return NULL;
}

/* Allows the AI to promote to a queen when possible */
void promotionAI(PIECE *p, MOVE *m, BOARD *b, PLAYER *player)
{
	char* end = getEnd(m);
	int y = getY(end);

	if ( (y == 7) && (getType(p) == PAWN) && (validMove(p, m, b, player) || captureMove(p, m, b, player)) ){
		printf("\nThe AI chooses to promote their pawn to a queen!\n");
		changeType(p, "queen");
	}
}
