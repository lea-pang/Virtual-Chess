/* humanGame.c : C file for human v. human game */
#include "humanGame.h"

/* Goes through the human v. human game */
void Human_v_Human(PLAYER *p1, PLAYER *p2, BOARD *b)
{
	//50 move rule integer counter
	int turns = 0;

	/* Move input strings */
	char input[5] = "";
	char start[3] = "";
	char* user_input = NULL;

	/* Prints board */
	printBoard(b);

	/* Keep track of turns */
	bool player1 = true;
	bool player2 = false;

	/* Boolean for valid move */
	bool valid = false;

	/* Boolean for if there's an enemy piece then valid capture */
	bool capture = false;
	bool c = false;
	while (true){
		if (player1){
	//check at the beginning of a turn if the current player is in check
        if((checkForCheck(b,p1,p2) != NULL)){
			if(checkForCheckmate(b, p1, p2) == true){
				printf("\nPlayer 2 wins!\n");
				break;
			}
               printf("\nPlayer 1, you are in check.\n");
         }
        
			/* Asks user to input a move like "A2A3" */
			printf("\nPlayer 1, Input your move: ");

			user_input = moveInput(b);
			input[0] = user_input[0];
			input[1] = user_input[1];
			input[2] = user_input[2];
			input[3] = user_input[3];

         free(user_input);

			if (strcmp(input, "exit") == 0){ // Player chooses to exit the game early
				break;
			}

			/* Parses user input */
			start[0] = input[0];
			start[1] = input[1];
         
			/* Initializes temporary PIECE *p to piece at the start location */
			PIECE *p = getPieceOnBoard(b, start);

			/* Creates new move based on user input */
			MOVE *m = createMove(p, input);
         
			/*get end piece*/
			char* end = getEnd(m);
			PIECE *pe = getPieceOnBoard(b, end);

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
			
			promotionMove(p, m, b, p1);

			/* if the capture is there, it will update the board, and the piece will be at the end location (skipping valid location)*/
			if(capture == true){
				// Changes location of a piece when a capture occurs
				printf("\nValid capture.\n");
				changePieceLocation(m, p);
				updateBoard(m,b);
				printBoard(b);

				//if move was valid, add move counter to the piece	
		            	addMove(p);

				addNode(m);
				deletePiece(pe);
				//resets the 50 move rule back to 0
				turns = 0;
				player1 = false;
				player2 = true;
			}

			/*needs capture is NOT true to go check if valid move + updating the board*/
			else if((valid == true) && (capture != true)){
				printf("\nValid move.\n");

				if (enPassant(p, m, b))
					enPassantCapture(p, m, b);

				if (castlingCondition(p, m, b))
						castling(m, b);

				changePieceLocation(m, p); /* Changes current location attribute of piece*/				 
				
				//if the move is valid, add a move counter to the piece
				addMove(p);

				updateBoard(m, b); /* Updates the current board using the move */
				printBoard(b);

            			addNode(m);

				//adds a turn if it is a move was made NOT by pawn
				if(getType(p) != PAWN)
				{
					//adds a turn 
					turns++;
				}
				else
					turns = 0;

				player1 = false;
				player2 = true;
			}	

         		//if both valid and capture are false, check if in checkmate or not
         		//if checkmate is false, player has to make a move to get out of check
			else {
             if(checkForCheck(b,p1,p2) != NULL){
               if(checkForCheckmate(b,p1,p2) == true){
                  break;
               }
              }    
			     printf("\nInvalid move.\n");
               if(m!=NULL)
                  deleteMove(m);
		}
      	//reset values at the end of the turn
      	valid = false;
      	capture = false;
      	c = false;


      }

	//stalemate
	if(checkForSM(b,p2,p1))
	{
		printf("\nThis game has ended with a stalemate.\n");
		break;
	}

	//insufficient material
	else if(checkForIM(b,p1,p2))
	{
		printf("\nThis game has ended with a stalemate due to insufficient material.\n\n");
		break;
	}

	//if greater than the 50 move rule, it will ask the user if they want to tie
	if(turns > 100)
	{
		bool claim = false;
		claim = claimTie();
		if(claim)
			break;
		else
			continue;
	}
   
	if (player2){
	         //checks if current player is in check or not
        	 if((checkForCheck(b,p2,p1) != NULL)){
			if (checkForCheckmate(b, p2, p1) == true){
				printf("\nPlayer 1 wins!\n");
				break;
			}
                   	printf("\nPlayer 2, you are in check.\n");
         	}

        	/* Asks user to input a move like "A2A3" */
			printf("\nPlayer 2, Input your move: ");

			user_input = moveInput(b);
			input[0] = user_input[0];
			input[1] = user_input[1];
			input[2] = user_input[2];
			input[3] = user_input[3];
         
         free(user_input);

			if (strcmp(input, "exit") == 0){ // Player chooses to exit the game early
				break;
			}
	
			/* Parses user input */
			start[0] = input[0];
			start[1] = input[1];

			/* Initializes temporary PIECE *p to piece at the start location */
			PIECE *p = getPieceOnBoard(b, start);

			/* Creates new move based on user input */
			MOVE *m;
			m = createMove(p, input);

			/*get end piece*/
			char* end = getEnd(m);
			PIECE *pe = getPieceOnBoard(b, end);

			/*if the end location is filled, it will check if it's an enemy space */
			if(!emptySpace(m,b))
				c = enemySpace(p2, pe, m, b);
			
			/*if there's an enemy in the space, it will check for capture, 
 			 * else if it's not enemy, it will check if it's a valid move*/ 
			if (c){
				capture = captureMove(p,m,b,p2);
          			//updates board based on input capture
            			changePieceLocation(m,p);
            			updateBoard(m,b);
            			//if, after updating board, player is still in check
            			//capture is false, and board is reset
            			if(checkForCheck(b,p2,p1) != NULL){
               				capture = false;
               				withdrawMove(p,b,m, pe);
            			}
            			//reset board if regular move
            			else{
               				withdrawMove(p,b,m, pe);
            			}
         		}

			else{
				valid = validMove(p,m,b,p2);
		                //updates board based on input valid move
		                changePieceLocation(m,p);
			        updateBoard(m,b);
        			//if. after updating board, player is still in check
        			//valid is false, and board is reset
        			if(checkForCheck(b,p2,p1) != NULL){
               				valid = false;
               				withdrawMove(p,b,m, pe);
            			}
            			//reset board if regualr move
            			else{
               				withdrawMove(p,b,m, pe);
            			}
			}

			/* Check for possible promotion */
			promotionMove(p, m, b, p2);

			/* if the capture is there, it will update the board, and the piece will be at the end location (skipping valid location)*/
			if(capture == true){
				printf("\nValid capture.\n");
				//changes location of piece bc there's a capture
				changePieceLocation(m, p);
				updateBoard(m,b);
				printBoard(b);
            addNode(m);
				
				//if the move is valid, add a move counter to the piece
				addMove(p);
            deletePiece(pe);
				//reset the 50 moves rules 
				turns = 0;
				player2 = false;
				player1 = true;
			}

			else if((valid == true) && (capture != true)){
				printf("\nValid move.\n");
				if (enPassant(p, m, b)) // enpassant
					enPassantCapture(p, m, b);

				if (castlingCondition(p, m, b)) //castling
						castling(m, b);

				changePieceLocation(m, p); /* Changes current location attribute of piece*/
				updateBoard(m, b); /* Updates the current board using the move */
				printBoard(b);
				addNode(m);

				//if the move is valid, add a move counter to the piece
				addMove(p);

				//adds a turn if it is a move was made NOT by pawn
				if(getType(p) != PAWN)
				{
					//adds a turn 
					turns++;
				}
				else
					turns = 0;
				player2 = false;
				player1 = true;
			}	
         
         //if valid and capture are both false check if in checkmate
         //if not in checkmate player has to make a valid move to get out of check
			else{
            if(checkForCheck(b,p2,p1) != NULL){
               if(checkForCheckmate(b,p2,p1) == true)
                  break;
            }
				printf("\nInvalid move.\n");
            if(m != NULL)
               deleteMove(m);
      	}
      //reset values at the end of the turn
      valid = false;
      capture = false;
      c = false;

		}

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
	if(turns > 100)
	{
		bool claim = false;
		claim = claimTie();
		if(claim)
			break;
		else
			continue;
		
	}
   }
	// End of the game, save a log of moves and deallocate memory
   printToFile(p1,p2);
   printf("\nThe game has ended.\n\n");
   removeList();
}

// Withdraws the previous move made, and updates the board to how it used to be
void withdrawMove(PIECE* p,BOARD* b, MOVE* m1, PIECE *pe){
	char start[3] = "";
	start[0] = getStart(m1)[0];
	start[1] = getStart(m1)[1];

	char end[3] = "";
	end[0] = getEnd(m1)[0];
	end[1] = getEnd(m1)[1];

   char movLocation[5] = "";
	movLocation[0] = end[0];
	movLocation[1] = end[1];
	movLocation[2] = start[0];
	movLocation[3] = start[1];
	MOVE* m = createMove(p,movLocation);

	changePieceLocation(m,p);

	updateBoard(m,b); // Replaces PIECE *p to its original start location to reverse a move
	reverseCapture(pe, b, end); // If a piece was captured, will place the captured piece back or leave it null

	deleteMove(m);
}

//prompts the user to ask if they want to declare a draw/tie
bool claimTie()
{
	char tie_input[30] = "";

	printf("Would you like to claim a tie here? Input y for yes and n for no: ");

	while (1){
		scanf("%s", tie_input);

		//if the inputs are not y or n, the game will ask him to input something valid
		if (strcmp(tie_input, "y") == 0)
			return true;

		else if (strcmp(tie_input, "n") == 0)
			return false;

		else{
			printf("\nPlease input a valid choice: ");
		}
	}
}
