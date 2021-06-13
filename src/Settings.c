/* Settings.h : Header file for settings */

/* Header files */
#include "Settings.h"

/* Takes user input and allows player to make changes in settings */
void Settings(PLAYER *p1, PLAYER *p2)
{
	char choice[30] = "";
	bool settings = 1;

	while (settings){
		printMenuSettings();
		scanf("%s", choice);

		if (strcmp(choice, "1") == 0){
			changePlayerColor(p1);
			changePlayerColor(p2);
			if (getPlayerColor(p1) == 'b'){
				printf("\nBlack is now Player 1. White is now Player 2.\n");
			}
			else{
				printf("\nWhite is now Player 1. Black is now Player 2.\n");
			}
			choice[0] = '\0';
			continue;
		}

		else if (strcmp(choice, "2") == 0){
			choice[0] = '\0';
			printf("\n");
			settings = 0;
			continue;
		}

		else{
			choice[0] = '\0';
			printf("\nPlease enter a valid input!\n");
			continue;
		}
	}
}

/* Prints the settings menu */
void printMenuSettings()
{
	printf("\nSettings Menu\n-------------\n");
	printf("1. Change player colors\n");
	printf("2. Exit settings\n");
	printf("Please make your choice: ");
}
