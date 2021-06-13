/* Board.h : Header file for Board */
#include "Board.h"

// Print the board out to the terminal for now
void printBoard(BOARD* b){
   printf("%s","\n -------------------------\n");
   int col, row;
   int side_int = 8;
   char begin;
   for(row = 0; row < 8 ; row++){
      printf("%d|",side_int);
      side_int--;
      for(col = 0; col < 8; col++){
         if(col != 0)
            printf("%c",'|');

         if(b->gameBoard[row][col] == NULL){
            printf("%s","  ");
			if (col == 7)
				printf("|");
		}
         
         else{
			printPiece(b->gameBoard[row][col]);
			if (col == 7)
				printf("|");
		}
   }
      if(row != 7){
         printf("%s","\n -------------------------\n");
         continue;
      }               
   }

   printf("%s","\n -------------------------\n");  
   for(begin = (char)(65); begin <= (char)(72); begin++){
		printf("  %c",begin);
   }
	printf("\n");
}

// Create Board to be prossessed 
BOARD* createBoard(){
   BOARD* board = malloc(sizeof(BOARD));
   
   resetBoard(board);
   return board;  
}

// Reset board to the original starting postion
void resetBoard(BOARD* b){
   b->gameBoard[0][0] = createPiece("rook",'b',"A8");
   b->gameBoard[0][1] = createPiece("knight",'b',"B8");
   b->gameBoard[0][2] = createPiece("bishop",'b',"C8");
   b->gameBoard[0][3] = createPiece("queen",'b',"D8");
   b->gameBoard[0][4] = createPiece("king",'b',"E8");
   b->gameBoard[0][5] = createPiece("bishop",'b',"F8");
   b->gameBoard[0][6] = createPiece("knight",'b',"G8");
   b->gameBoard[0][7] = createPiece("rook",'b',"H8");
   b->gameBoard[1][0] = createPiece("pawn",'b',"A7");
   b->gameBoard[1][1] = createPiece("pawn",'b',"B7");
   b->gameBoard[1][2] = createPiece("pawn",'b',"C7");
   b->gameBoard[1][3] = createPiece("pawn",'b',"D7");
   b->gameBoard[1][4] = createPiece("pawn",'b',"E7");
   b->gameBoard[1][5] = createPiece("pawn",'b',"F7");
   b->gameBoard[1][6] = createPiece("pawn",'b',"G7");
   b->gameBoard[1][7] = createPiece("pawn",'b',"H7");
   

   b->gameBoard[7][0] = createPiece("rook",'w',"A1");
   b->gameBoard[7][1] = createPiece("knight",'w',"B1");
   b->gameBoard[7][2] = createPiece("bishop",'w',"C1");
   b->gameBoard[7][3] = createPiece("queen",'w',"D1");
   b->gameBoard[7][4] = createPiece("king",'w',"E1");
   b->gameBoard[7][5] = createPiece("bishop",'w',"F1");
   b->gameBoard[7][6] = createPiece("knight",'w',"G1");
   b->gameBoard[7][7] = createPiece("rook",'w',"H1");
   b->gameBoard[6][0] = createPiece("pawn",'w',"A2");
   b->gameBoard[6][1] = createPiece("pawn",'w',"B2");
   b->gameBoard[6][2] = createPiece("pawn",'w',"C2");
   b->gameBoard[6][3] = createPiece("pawn",'w',"D2");
   b->gameBoard[6][4] = createPiece("pawn",'w',"E2");
   b->gameBoard[6][5] = createPiece("pawn",'w',"F2");
   b->gameBoard[6][6] = createPiece("pawn",'w',"G2");
   b->gameBoard[6][7] = createPiece("pawn",'w',"H2");
   int row;
   for(row = 2;row<6;row++){
      b->gameBoard[row][0] = NULL;
      b->gameBoard[row][1] = NULL;
      b->gameBoard[row][2] = NULL;
      b->gameBoard[row][3] = NULL;
      b->gameBoard[row][4] = NULL;
      b->gameBoard[row][5] = NULL;
      b->gameBoard[row][6] = NULL;
      b->gameBoard[row][7] = NULL;
	}
}

PIECE* getPieceOnBoard(BOARD *b, char l[3])
{
	int col = l[0] - 65;
	int row = 8 - (l[1] - 48);
	
	if ((b->gameBoard[row][col]) != NULL)
		return b->gameBoard[row][col];
	else
		return NULL;
}

//deallocated memory storing BOARD *b
void deleteBoard(BOARD* b){

   for(int r = 0; r < 8;r++){
      for( int c = 0; c < 8;c++){
         if(b->gameBoard[r][c] != NULL){
            deletePiece(b->gameBoard[r][c]);
         }
      }
   }
   free(b);
}
