/*Authors:
 * Stella Zhou
 *
 * Updates:
 * 05/02/21 SZ fixed formatting + added comments + debugged a few AI bounds
 * 04/29/21 SZ added in beginner AI (first available move)
 * 04/20/21 SZ included all the functions necessary in AIgame.h
 */

//Human v AI game 
void Human_v_AI(PLAYER *p1, PLAYER *p2, BOARD *b);
//prints the AI menu
void AIMenu();
//confirms with player if they want to make this move
bool confirmMove();

//finds a random piece and location to go to
int randAI(BOARD *b, PLAYER *p2, PLAYER *p1);

//finds the first available move 
int begAI(BOARD *b, PLAYER *p2, PLAYER *p1);

//updates the board and player information if move was valid
int playerMove(PIECE *p, MOVE *m, BOARD *b, PLAYER *p1, PLAYER *p2, PIECE *pe);

//finds the inital random piece (that random AI found)
PIECE* rPieceOnBoard(BOARD *b, int row, int col);

// checks if one of the AI's pawns can promote after it makes a valid move
void promotionAI(PIECE *p, MOVE *m, BOARD *b, PLAYER *player);
