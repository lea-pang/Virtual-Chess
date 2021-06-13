/* Main.c : C File for main menu */
#include "Piece.h"
#include "Board.h"
#include "Move.h"
#include "Player.h"
#include "humanGame.h"
#include "Settings.h"
#include "AIgame.h"
#include "Win.h"
#include "client.h"
#include "onlineGame.h"
#include "Messages.h"

#include <stdbool.h>
#include <string.h>

void PrintMenuGame();

void PrintOnlineMenu();

int main(int argc, char *argv[])
{
	// Log in
	printf("\nWelcome to Virtual Chess!\n-------------------------\n");

   char* username_login = NULL;
   char username[256];
   bzero(username,256);

	/* Input char pointer */
	char* choice = NULL;
	char* onlineChoice = NULL;
	int sockfd = 0;

	/* Creates player */
	PLAYER *p1;
	p1 = createPlayer('w', 1);
	PLAYER *p2;
	p2 = createPlayer('b', 2);

	/* Creates the board */
	BOARD *b;
	b = createBoard();
	
	/* While loop boolean */
	bool menu = 1;
    char input[256], onlineInput[256];

	/* Validate user input and call functions */
	while (menu){
	  PrintMenuGame();

      bzero(input,256);
      fgets(input,256,stdin);

	  if (strcmp(input, "\n") == 0){
		bzero(input, 256);
		fgets(input, 256, stdin);
	  }

      choice = my_strdup(input);
      choice[strlen(choice) - 1] = '\0';

		if (strcmp(choice, "1") == 0){
			Human_v_Human(p1, p2, b);
			deleteBoard(b);		
			b = NULL;
			b = createBoard();
			*choice = '\0';
		}

		else if (strcmp(choice , "2") == 0){
			Human_v_AI(p1, p2, b);
			deleteBoard(b);
			b = NULL;
			b = createBoard();
			*choice  = '\0';
		}
		else if (strcmp(choice, "3") == 0){
			struct sockaddr_in server = serverAdd(argc, argv);
			sockfd = dataSocket(argc, argv);

			if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
				error("ERROR connecting");
			
			if(username_login == NULL){
	         username_login = login(sockfd, argc, argv); // login
	         strncpy(username, username_login, strlen(username_login)); // save username
	         username[strlen(username) - 1] = '\0'; // Remove newline character
 			}

	        // Debugging
	        printf("*** Your username is: %s ***\n", username);

	        bool onlineMenu = true;

			while (onlineMenu){
				PrintOnlineMenu();
				bzero(onlineInput, 256);
				fgets(onlineInput, 256, stdin);

		  		if (strcmp(onlineInput, "\n") == 0){
					bzero(onlineInput, 256);
					fgets(onlineInput, 256, stdin);
	  			}

				onlineChoice = my_strdup(onlineInput);
				onlineChoice[strlen(onlineChoice) - 1] = '\0';

				if (strcmp(onlineChoice, "1") == 0){
					if (sendGameRequests(sockfd, argc, argv, username) == false){ // no one to sent requests to
						printf("\nYou will be using the white pieces.\n");
						if (getPlayerColor(p1) == 'b'){
							changePlayerColor(p1);
							changePlayerColor(p2);
						}
					}

					else{
                  printf("\nYou will be using the black pieces.\n");
						if (getPlayerColor(p1) == 'w'){
							changePlayerColor(p1);
							changePlayerColor(p2);
						}
					}

					Human_v_Human_Online(sockfd, argc, argv, p1, p2, b);
					clientExit(sockfd, argc, argv, username);

					deleteBoard(b);
					b = NULL;
					b = createBoard();
					*onlineChoice = '\0';
				}
			
				else if (strcmp(onlineChoice, "2") == 0){
					if (sendGameRequests(sockfd, argc, argv, username) == false){
						if (getPlayerColor(p1) == 'b'){
							changePlayerColor(p1);
							changePlayerColor(p2);
						}
					}
					else{
						if (getPlayerColor(p1) == 'w'){
							changePlayerColor(p1);
							changePlayerColor(p2);
						}
					}
					msgMain(sockfd, p1, p2);
					clientExit(sockfd, argc, argv, username);				

					*onlineChoice = '\0';
				}

            else if (strcmp(onlineChoice, "3") == 0){
               shutDown(sockfd);
               deleteBoard(b);
					b = NULL;
					b = createBoard();
					*onlineChoice = '\0';
					onlineMenu = false;
            }

				else{
					printf("\nReturning to the main menu.\n\n");
					clientExit(sockfd, argc, argv, username);
					close(sockfd);
					deleteBoard(b);
					b = NULL;
					b = createBoard();
					*onlineChoice = '\0';
					onlineMenu = false;
				}
			}
		}

		else if (strcmp(choice, "4") == 0){
			Settings(p1, p2);
			*choice = '\0';
		}
		
		else if (strcmp(choice, "5") == 0){
			menu = false;
			printf("\nThank you for playing!\n");
			*choice = '\0';
		}

		else{
			printf("\nPlease input something valid!\n\n");
			*choice = '\0';			
		}
	}

	/* Deallocate memory of players and board */
	deletePlayer(p1);
	p1 = NULL;
	deletePlayer(p2);
	p2 = NULL;
	deleteBoard(b);
   	b = NULL;
	free(choice);
	choice = NULL;

	close(sockfd);

	return 0;
}

void PrintMenuGame()
{
	printf("Main Menu\n---------\n");
	printf("1: Human v Human\n");
	printf("2: Human v AI\n");
	printf("3: Online Human v Human\n");
	printf("4: Change settings\n");
	printf("5: Exit game\n");
	printf("Please make your choice: ");
}

void PrintOnlineMenu()
{
	printf("Online Main Menu\n----------------\n");
	printf("1: Start an online game\n");
	printf("2: Message\n");
   printf("3: Shutdown Server\n");
	printf("4: Exit to main menu\n");
	printf("Please make your choice: ");
}
