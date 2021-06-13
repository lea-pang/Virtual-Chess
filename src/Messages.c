#include "Messages.h"
#include "onlineGame.h"
#include "client.h"
#include <stdbool.h>

/* the main messaging SENDING skeleton */
int sendMessage(int sockfd)
{
   int n;
   char sendbuffer[256];
	char message[256];
	char buffer[256];
	char* compare;
   char accesscode[7] = "MESSG";

   n = write(sockfd, accesscode, strlen(accesscode)); /* Writes message accesscode */
   if(n<0) error("ERROR writing to socket");

   bzero(buffer, 256);
   n = read(sockfd, buffer, 255); // Status message from server

   printf("\nYou: ");
	bzero(sendbuffer,256);
	fgets(sendbuffer, 255, stdin);
	bzero(message, 256);
	strcat(message, sendbuffer);

	compare = strstr(message, "exit"); // Check if sent message is "exit"
	n = write(sockfd, message, strlen(message));

	//Check if exit was entered
	if(compare != NULL)
		return true;
	else
   		return false; 

}
/* checking if there's an UNOPEN message from a client */
/* viewing or receiving a message? */
char* receiveMessage(int sockfd)
{
	char buffer[256];
	int n;

	char* noneToSend = NULL;
	char none[5] = "NONE";
	noneToSend = none;

	char* exitChat = NULL;
	char exit[5] = "exit";
	exitChat = exit;

	char* compare = NULL;
	char accesscode[7] = "REMSG";

	n = write(sockfd, accesscode, strlen(accesscode));

	if (n < 0) error("ERROR writing to socket");

	bzero(buffer, 256);

	//should be reading the message received from the server
    n = read(sockfd, buffer, 255);
	if (n < 0) error("ERROR reading from socket");

	compare = strstr(buffer, "exit"); // check if received message is "exit"

	if (strcmp(buffer, "There is no msg.\n") == 0){ // other player hasn't sent a message
		return strdup(noneToSend);
	}

	else if (compare != 0){ // other player entered exit
		return strdup(exitChat);
	}

	else{
		printf("\nMessage received: %s", buffer); // print the received message
		return strdup(buffer);
	}
}

/* specific chat log skeleton for choice #4 */
void msgMain(int sockfd, PLAYER *p1, PLAYER *p2)
{
	int loop = 0;
	bool player1 = (getPlayerColor(p1) == 'w');
	bool player2 = (getPlayerColor(p1) == 'b');
	printf("------- Welcome to online chat! ------\n");
	char* user_input = NULL;
	bool msg = true;

	while(msg)
	{
		if(player1){
			loop = sendMessage(sockfd);
			if(loop == 1){
				printf("\nYou have left the chat.\n\n");
				break;
			}
			while(true){
				user_input = receiveMessage(sockfd);
				if (strcmp(user_input, "NONE") != 0){
					break;
				}
			}
			if (strcmp(user_input, "exit") == 0){
				printf("\nThe other user has left the chat.\n\n");
				break;
			}
		}

		if(player2){
			while(true){
				user_input = receiveMessage(sockfd);
				if (strcmp(user_input, "NONE") != 0){
					break;
				}
			}

			if (strcmp(user_input, "exit") == 0){
				printf("\nThe other user has left the chat.\n\n");
				break;
			}

			loop = sendMessage(sockfd);
			if (loop == 1){
				printf("\nYou have left the chat.\n\n");
				break;
			}
		}
	}

}

