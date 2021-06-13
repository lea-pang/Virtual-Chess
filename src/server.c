/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/select.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <arpa/inet.h>
#include <assert.h>

/* Struct */
typedef void (*ClientHandler)(int DataSocketFD);
typedef void (*TimeoutHandler)(void);

/* Global variables */
const char* Program = NULL;
int Shutdown = 0;
char ClockBuffer[26] = "";
int socket1 = 0;
int socket2 = 0;
char tempMovePlayer1[5] = "";
char tempMovePlayer2[5] = "";
char msg1[256] = "";
char msg2[256] = "";

/*Available Player List*/
typedef struct UserNode{
   char *username;
	struct UserNode* next;
   struct UserNode* prev;
} UserNode;

static struct UserNode *head = NULL;
static struct UserNode *last = NULL;

/* Function declarations */
char* my_strdup(const char* s);

int MakeServerSocket(uint16_t PortNo);

void ProcessRequest(int DataSocketFD);

void LoginLoop(int ServerSocketFD, ClientHandler HandleClient, TimeoutHandler HandleTimeout, int Timeout);

void printToServerFile(char user[256]);

bool checkIfAddressExists(char address[256]);

bool checkIfPasswordMatches(char user[256], char pass[256]);

void recMsg(int DataSocketFD);

bool checkMatch(char user[256], char address[256]);

void PrintCurrentTime();

void receiveMove(int DataSocketFD);

void sendMove(int DataSocketFD);

void sendRequest(int DataSocketFD);

void sendMsg(int DataSocketFD);

void recMsg(int DataSocketFD);

void login(int DataSocketFD);

bool isListEmpty();

void insertNodeLast(char *user,int userLen);

void insertNodeFirst(char *user,int userLen);

void displayList();

UserNode* makeNode(char user[256]);

int compareNode(struct UserNode* s1, struct UserNode* s2);

struct UserNode *addNode(struct UserNode *list, struct UserNode *node);

void removeList();

bool search(UserNode* head, char user[256]);

void onlineExit(int DataSocketFD);

int deleteNode(char user[256]);

struct UserNode* userNode(char *user);

/* Allocates memory for char pointer */
char* my_strdup(const char* s)
{
	char* new;
	size_t len = strlen(s) + 2;
	new = malloc(len);
	
	if (new){
		return (char*)memcpy(new,s,len);
	}
	return NULL;
}

/* Prints out an error messsage */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/* Main for server */
int main(int argc, char *argv[])
{
   int sockfd, PortNo;
     
   Program = argv[0];

   if (argc < 2) {
       fprintf(stderr,"ERROR, no port provided\n");
       exit(1);
   }

   PortNo = atoi(argv[1]);

   if(PortNo <= 2000){
      fprintf(stderr,"%s: invalid port number %d, should be greater than 2000\n",Program,PortNo);
      exit(1);
   } 

   sockfd = MakeServerSocket(PortNo);
   printf("%s: Providing chess access at port %d\n",Program,PortNo);

	/* Login */

   LoginLoop(sockfd,ProcessRequest,PrintCurrentTime,250000);

   printf("\n%s:Shutting down.\n",Program);
   close(sockfd);
   return 0; /* we never get here */
}

/* Prints the current time */
void PrintCurrentTime(void){
   fflush(stdout);
}

/* Creates a server socket */
int MakeServerSocket(uint16_t PortNo){
   int sockfd;
   struct sockaddr_in ServerSocketName;

   sockfd = socket(PF_INET, SOCK_STREAM,0);
   if(sockfd < 0){
      error("socket creation failed");
   }
   ServerSocketName.sin_family = AF_INET;
   ServerSocketName.sin_port = htons(PortNo);

   ServerSocketName.sin_addr.s_addr = htonl(INADDR_ANY); /* IP address */

   if(bind(sockfd,(struct sockaddr*)&ServerSocketName,sizeof(ServerSocketName)) <0){
      error("binding to socket failed");
   }  
   if(listen(sockfd,5) <0){
      error("listening on socket failed");
   }
   return sockfd;
}

/* Processes client request */
void ProcessRequest(int DataSocketFD){
   int l,n;
   char RecvBuf[7];
   char SendBuf[256];

   bzero(RecvBuf, 7);
   n = read(DataSocketFD,RecvBuf,sizeof(RecvBuf)-1);
   if(n<0)
      error("reading data from socket failed");
   RecvBuf[n] = 0;

   if(0 == strcmp(RecvBuf,"LOGIN")){
      strncpy(SendBuf,"Starting Login\n",sizeof(SendBuf)-1);
      SendBuf[sizeof(SendBuf)-1] = 0;
      l = strlen(SendBuf);
      n = write(DataSocketFD, SendBuf, l);
      login(DataSocketFD);
	}
   else if(0 == strcmp(RecvBuf, "SENDR")) {
      strncpy(SendBuf, "Sending Request\n",sizeof(SendBuf)-1);
      SendBuf[sizeof(SendBuf)-1] = 0;
      l = strlen(SendBuf);
      n = write(DataSocketFD, SendBuf, l);
      sendRequest(DataSocketFD);
   }
   else if (0 == strcmp(RecvBuf, "MOVES")) {
      strncpy(SendBuf, "Sending your move\n",sizeof(SendBuf)-1);
      SendBuf[sizeof(SendBuf)-1] = 0;
      l = strlen(SendBuf);
      n = write(DataSocketFD, SendBuf, l);
      sendMove(DataSocketFD);
	}
   else if (0 == strcmp(RecvBuf, "MOVER")) {
      receiveMove(DataSocketFD);
	}
	else if (0 == strcmp(RecvBuf, "MESSG")){
		strncpy(SendBuf, "Message sent\n", sizeof(SendBuf)-1);
		SendBuf[sizeof(SendBuf)-1] = 0;
		l = strlen(SendBuf);
		n = write(DataSocketFD, SendBuf, l);
		sendMsg(DataSocketFD);
	}
	else if (0 == strcmp(RecvBuf, "REMSG")){
		recMsg(DataSocketFD);
	}
	else if (0 == strcmp(RecvBuf, "EXITS")) {
	  strncpy(SendBuf, "Exiting game session\n", sizeof(SendBuf)-1);
	  SendBuf[sizeof(SendBuf)-1] = 0;
	  l = strlen(SendBuf);
	  n = write(DataSocketFD, SendBuf, l);
	  onlineExit(DataSocketFD);
	}

   else if(0 == strcmp(RecvBuf,"SHUTDN")){
     Shutdown = 1;
     strncpy(SendBuf, "Shutting down server\n", sizeof(SendBuf)-1);
	  SendBuf[sizeof(SendBuf)-1] = 0;
	  l = strlen(SendBuf);
	  n = write(DataSocketFD, SendBuf, l);
   }
}

void onlineExit(int DataSocketFD) /* Client wants to exit online game mode */
{
	char username[256];
	int n;
	char SendBuf[256];

	bzero(username, 256);
	n = read(DataSocketFD, username, 255); // Reads username from socket
	printf("\nPlayer that has exited: %s", username);
	if (n < 0) error("ERROR reading username from socket");

	deleteNode(username);

	socket1 = 0;
	socket2 = 0;
	bzero(tempMovePlayer1, 5);
	bzero(tempMovePlayer2, 5); // Reset all global variables

	char exit[7] = "Exited"; // Send status message to client
	strncpy(SendBuf, exit, sizeof(exit)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
	int l = strlen(SendBuf);
	n = write(DataSocketFD, SendBuf, l);
	if (n < 0) error("ERROR writing to socket");
}

/* Delete UserNode given a username */
int deleteNode(char user[256])
{
	UserNode* current = head;
	UserNode* next = NULL;
	char *comp;

	if (head == NULL)
		return false;

	while (current != NULL){
		comp = strstr(user, current->username); // Check if username of client matches a node

		if (comp != NULL){
			next = current->next;
			if (head == NULL || current == NULL){
				current = next;
				continue;
			}
			else if (head == current){
				head = current->next;
			}
			else if (current->next != NULL){
				current->next->prev = current->prev;
			}
			else if (current->prev != NULL){
				current->prev->next = current->next;
			}
			free(current->username);
			free(current);
			current = next;
		}
		else{
			current = current->next;
		}
	}
	return true;
}

void sendMove(int DataSocketFD) /* Client wants to send a move */
{
	char buffer[256];
	int n;

	bzero(buffer, 256);

	n = read(DataSocketFD, buffer, 255); /* Contains the move */
	if (n < 0) error("ERROR reading from socket");

	if (DataSocketFD == socket1){ // player 1 is sending a move
		tempMovePlayer1[0] = buffer[0];
		tempMovePlayer1[1] = buffer[1];
		tempMovePlayer1[2] = buffer[2];
		tempMovePlayer1[3] = buffer[3];
		tempMovePlayer1[4] = '\0';
		printf("\n\nReceived move is: %s\n", tempMovePlayer1);
	}

	else if (DataSocketFD == socket2){ // player 2 is sending a move
		tempMovePlayer2[0] = buffer[0];
		tempMovePlayer2[1] = buffer[1];
		tempMovePlayer2[2] = buffer[2];
		tempMovePlayer2[3] = buffer[3];
		tempMovePlayer2[4] = '\0';
		printf("\n\nReceived move is: %s\n", tempMovePlayer2);
	}

	else{
		error("ERROR receiving move from client");
	}
}

void sendMsg(int DataSocketFD) /* Client wants to send message */
{
	char buffer[256];
	int n, i;

	bzero(buffer, 256);
	n = read(DataSocketFD, buffer, 255); // Contains the message
	if (n < 0) error("ERROR reading from socket");

	if (DataSocketFD == socket1){ // player 1 is sending a message
		for(i = 0; i < strlen(buffer); i++)
		{
			msg1[i] = buffer[i];
		}
		msg1[strlen(buffer)] = '\0';
		printf("\n\nReceived msg is: %s\n", msg1);
	}

	else if (DataSocketFD == socket2){ // player 2 is sending a message
		for(i = 0; i < strlen(buffer); i++)
		{
			msg2[i] = buffer[i];
		}
		msg2[strlen(buffer)] = '\0';
		printf("\n\nReceived msg is: %s\n", msg2);
	}

	else{
		error("ERROR receiving move from client");
	}
}

void recMsg(int DataSocketFD){/*client wants to receive message*/
	int n;
	if (DataSocketFD == socket1){
		if (msg2[0] != '\0'){
			n = write(DataSocketFD, msg2, 255);
			bzero(msg2, 256);
		}
		else{
			n = write(DataSocketFD, "There is no msg.\n", 20); // send player 1 player 2's message
		}
	}
	else if (DataSocketFD == socket2){
		if (msg1[0] != '\0'){
			n = write(DataSocketFD, msg1, 255); // send player 2 player 1's message
			bzero(msg1, 256);
		}
		else{
			n = write(DataSocketFD, "There is no msg.\n", 20);
		}
	}
	else{
		n = write(DataSocketFD, "There is no msg.\n", 20);
	}
	if (n < 0) error("ERROR receiving message from client");
}
	
void receiveMove(int DataSocketFD){
	int n;
	if (DataSocketFD == socket1){ // player 1 is receiving a move
		if (tempMovePlayer2[0] != '\0'){
			n = write(DataSocketFD, tempMovePlayer2, strlen(tempMovePlayer2));
			bzero(tempMovePlayer2, 5);
		}
		else{
			n = write(DataSocketFD, "There is no move.\n", 20);
		}
	}

	else if (DataSocketFD == socket2){ // player 2 is receiving a move
		if (tempMovePlayer1[0] != '\0'){
			n = write(DataSocketFD, tempMovePlayer1, strlen(tempMovePlayer1));
			bzero(tempMovePlayer1, 5);
		}
		else
			n = write(DataSocketFD, "There is no move.\n", 20);
	}

	else{
		n = write(DataSocketFD, "There is no move.\n", 20);
	}

	if (n < 0) error("ERROR sending move to client");
}

/* Loop for login process */
void LoginLoop(int ServerSocketFD, ClientHandler HandleClient, TimeoutHandler HandleTimeout, int Timeout){
   int DataSocketFD;
   socklen_t ClientLen;
   struct sockaddr_in ClientAddress;
   fd_set ActiveFD;
   fd_set ReadFD;
   struct timeval TimeVal;
   int res,i;
   
   FD_ZERO(&ActiveFD);
   FD_SET(ServerSocketFD,&ActiveFD);
   
   while(!Shutdown){
      ReadFD = ActiveFD;

      TimeVal.tv_sec = Timeout / 1000000;
      TimeVal.tv_usec = Timeout % 1000000;

      res = select(FD_SETSIZE, &ReadFD,NULL,NULL,&TimeVal);

      if(res < 0){
         error("wait for input or timeout failed");
      }

      if(res == 0){
         HandleTimeout();
      }

      else{
         for(i=0;i<FD_SETSIZE;i++){
            if(FD_ISSET(i,&ReadFD)){

               if(i == ServerSocketFD){
                  ClientLen = sizeof(ClientAddress);
                  DataSocketFD = accept(ServerSocketFD,(struct sockaddr*)&ClientAddress,&ClientLen);
                  if(DataSocketFD < 0){
                     error("Accept failed");
                  }
                  FD_SET(DataSocketFD,&ActiveFD);
               }
               else{
                  ProcessRequest(i);
               }
            }
         }
      }
    }
}

/* Process login inputs and verifies username/password matching */
void login(int DataSocketFD)
{
   int n,x;
   char username[256];
   char password[256];
	char address[256];

	/* Read IP address from socket */
   bzero(address,256);
   n = read(DataSocketFD, address, 255);
   if (n < 0) error("ERROR reading from socket");
	printf("\nHere is the IP address: %s\n", address);

	/* Notify client that IP address was received and connection was successful */
	n = write(DataSocketFD, "Successfully connected.\n\n", 50);

   if( (checkIfAddressExists(address) == true) ){ // IP Address has an account registered

	/* IP Address and username match check */
	  do{
		bzero(username, 256);
		while (0 == (x = read(DataSocketFD, username, 255)));
			if (checkMatch(username, address) == true){
				if (x < 0) error("ERROR reading from socket");
				printf("Here is the matching username: %s\n", username);
				x = write(DataSocketFD, "\nWelcome Back!\n", 50);
				if (x < 0) error ("ERROR writing to socket");
				break;
			}
			else{
				if (x < 0) error("ERROR reading from socket");
				printf("Here is the incorrect username: %s\n", username);
				x = write(DataSocketFD, "\nAddress has already been registered.\n", 50);
				if (x < 0) error("ERROR writing to socket");
			}
	  }while(checkMatch(username, address) != true);

	  x = write(DataSocketFD, "", 1);

	/* Username and password check */
	  do{
      	bzero(password,256);
		while(0 == (x = read(DataSocketFD, password, 255)));

      		if(checkIfPasswordMatches(username, password) == true){
      	   		if (x < 0) error("ERROR reading from socket");
      	   			printf("Here is the correct password: %s\n", password);
      
       	   	x = write(DataSocketFD,"\n\nYou have successfully logged in.\n", 50); 
      	   	if (x < 0) error("ERROR writing to socket");
			break;
      		}
      	else{
         	if (x < 0) error("ERROR reading from socket");
         	printf("Here is the incorrect password: %s\n", password);
      
         	x = write(DataSocketFD,"\n\nThat password was incorrect.\n", 50); 
         	if (x < 0) error("ERROR writing to socket");
      	}   
	  }while(checkIfPasswordMatches(username, password) != true);
   }
	
	// New account creation
   else{ 
	  printToServerFile(address); /* Append new IP Address */

	  // Obtain new username input
	  bzero(username, 256);
	  while (0 == (x = read(DataSocketFD, username, 255)));
	  if (x < 0) error("ERROR reading from socket");
	  printf("Here is the new username: %s\n", username);
	  x = write(DataSocketFD, "", 1);
	  if (x < 0) error("ERROR writing to socket");
	  printToServerFile(username);

	  x = write(DataSocketFD, "New username successfully entered.\n\n", 50);

	  // Obtain new password input
      bzero(password,256);
      while(0 ==(x = read(DataSocketFD,password,255)));
      if (x < 0) error("ERROR reading from socket");
      printf("Here is the new password: %s\n",password);
      x = write(DataSocketFD,"\nNew account successfully created.\n\n", 50); 
      if (x < 0) error("ERROR writing to socket");
      
      printToServerFile(password); /* Append new password */
   }
  
}

/* Prints char user[256] to account text file */
void printToServerFile(char user[256]){
   
   FILE *userNames = fopen("./bin/userNames.txt","a+");

   if(userNames == NULL){
      printf("ERROR could not open file\n");
      exit(-1);
   }
      
   for(int i = 0;i < 256;i++){
       if(user[i] == '\0'){
           break;
        }
        fprintf(userNames,"%c",user[i]);
   }
    fclose(userNames);
}

/* Checks if an IP address exists in account text file */
bool checkIfAddressExists(char address[256])
{
	FILE *userNames;
	int line = 1;
	int found = 0;
	char temp[512];

	if ((userNames = fopen("./bin/userNames.txt","r")) == NULL){
		return false;
	}

	while (fgets(temp, 512, userNames) != NULL){
		if (((strstr(temp, address)) != NULL) && (( (line + 2) % 3) == 0)){
			printf("Found IP address %s on line: %d\n", temp, line);
			found++;
		}
		if (found > 0){
			fclose(userNames);
			return true;
			break;
		}
		line++;
	}
	
	if (found == 0){
		printf("\nIP Address not found, this is a new user.\n");
		fclose(userNames);
		return false;
	}
	return false;
}

/* Checks if a username and password pair match in account text file */
bool checkIfPasswordMatches(char user[256], char pass[256]){
   FILE* userNames; 
   int found = 0;
	int line = 1;
   char temp[512];
   char password[512];
   char* passwordCheck;

   if((userNames = fopen("./bin/userNames.txt","r")) == NULL){
      return false;
   }

   while(fgets(temp,512,userNames) != NULL){
      if(((strstr(temp,user)) != NULL) && (( (line + 1) % 3) == 0)){ 
         found++;
    }
	  line++;

      if(found > 0){
         passwordCheck = fgets(password,512,userNames);
         if((strcmp(passwordCheck,pass)) == 0){
            fclose(userNames);
            return true;
         }
         else{
            fclose(userNames); 
            return false;
            }
      }
   }
   fclose(userNames);
   return false;
}

/* Check if IP Address corresponds to username */
bool checkMatch(char user[256], char address[256])
{
   FILE* userNames; 
   int found = 0;
	int line = 1;
   char temp[512];
   char username[512];
   char* usernameCheck;

   if((userNames = fopen("./bin/userNames.txt","r")) == NULL){
      return false;
   }

   while(fgets(temp,512,userNames) != NULL){
      if(((strstr(temp, address)) != NULL) && (( (line + 2) % 3) == 0)){ // found address
         found++;
      }
	  line++;

      if(found > 0){
         usernameCheck = fgets(username,512,userNames); // username after the address matches char user[256]
         if((strcmp(usernameCheck, user)) == 0){
            fclose(userNames);
            return true;
         }
         else{
            fclose(userNames); 
            return false;
         }
      }
   }

   fclose(userNames);
   return false;
}

/* Adds user to list of available players */
void sendRequest(int DataSocketFD)
{
   int n,x;
   char list[1024];
   char buffer[256];
	char playerUser[256];

   bzero(buffer, 256);
   n = read(DataSocketFD, buffer, 255);
   if (n < 0) error("ERROR reading from socket"); // Contains user sending the request
   printf("\n%s is sending a request\n", buffer);
  
	strcpy(playerUser, buffer);

	n = write(DataSocketFD, "Received username.\n", 21);
	if (n < 0) error("ERROR reading from socket");
	
	// Add user to available list of users
      int userLen = 0;
      /*compute the length of the username and address up to newline*/
      for(int i = 0;i<256;i++){
         if(playerUser[i] == '\n')
            break;
         userLen+=1;
      }
     /*if list is empty add node to the beginning else add it to the end*/
     if(isListEmpty()){
         insertNodeFirst(playerUser,userLen);
     }else{
         insertNodeLast(playerUser,userLen);
      }

	bzero(buffer, 256);
	n = read(DataSocketFD, buffer, 255);
	if (n < 0) error("ERROR reading from socket"); // Contains status message from client
	printf("\n%s\n", buffer);

	for (int i = 0; i < 256; i++){
		if (playerUser[i] == '\0')
			break;
		userLen += 1;
	}

	char* usernameStr = malloc(sizeof(char)*(userLen+1));
	memset(usernameStr, '\0', userLen+1);
	strncpy(usernameStr, playerUser, userLen + 1);
	usernameStr[userLen] = '\0';
	int count = 0;

    bzero(list, 1024); 

    struct UserNode* nodePtr = head;

	while ((nodePtr != NULL) && (strcmp(nodePtr->username, usernameStr) != 0)){ // Count number of available players
		strcat(list, nodePtr->username);
		count++;
		nodePtr = nodePtr->next;
	}

	if ((count == 0)){
		x = write(DataSocketFD, "No players available.\n", 22);
		if (x < 0) error("ERROR reading from socket");
		printf("\nTold client there is no players available.");
		// set global variables
		socket1 = DataSocketFD;
	}
	else{
		x = write(DataSocketFD, list, 1024);
		if (x < 0) error("ERROR reading from socket");
		printf("\nSent player list of available users: %s", list); 
		// set global variables
		socket2 = DataSocketFD;
   }
	free(usernameStr);
}

/*check if the list is empty or not*/
bool isListEmpty(){
   return head == NULL;
}

struct UserNode* userNode(char *user){
   struct UserNode* opp = head;
   
   if(isListEmpty()){
      return NULL;
   }
   
   while((strcmp(opp->username,user) != 0)&&(opp != NULL)){
      opp = opp->next;   
   }

   return opp;
}

/*Creates a node and inserts it at the beginning of the list given username and IP*/
void insertNodeFirst(char *user,int userLen){
   struct UserNode *node = (struct UserNode*)malloc(sizeof(struct UserNode));

   node->username = malloc(sizeof(char)*(userLen+1));
   memset(node->username,'\0',userLen+1);
   strncpy(node->username,user,userLen+1);
   node->username[userLen] = '\0';


   if(isListEmpty()){
      last = node;
   }
   else{
      head->prev = node;
   }

   node->next = head;
   head = node;
}

/*Creates a node and inserts it at the end of the list given the username and IP*/
void insertNodeLast(char *user,int userLen){
   struct UserNode *node = (struct UserNode*)malloc(sizeof(struct UserNode));

   node->username = malloc(sizeof(char)*(userLen+1));
   memset(node->username,'\0',userLen+1);
   strncpy(node->username,user,userLen+1);
   node->username[userLen] = '\0';


   if(isListEmpty()){
      last = node;
   }
   else{
      last->next = node;
      node->prev = last;
   }
   last = node;
}

/*Prints the current list of nodes each line is sperated by a new line, from the *head to *tail*/
void displayList(){
   struct UserNode *nodePtr = head;
   
   printf("\n [");
   
   while(nodePtr != NULL){
      printf(" ( %s\n ) ",nodePtr->username);
      nodePtr = nodePtr->next;
   }

   printf(" ]");
}

/*delete all the nodes in the list*/
void removeList() {
   struct UserNode *temp = head;
   struct UserNode *next;
   while (temp != NULL) {
	   next = temp->next;
	   temp->prev = NULL;
      free(temp->username);
      free(temp);
	   temp = next;
   }
}
 
