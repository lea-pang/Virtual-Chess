#include "client.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

const char *Program = NULL;
 
char* my_strdup(const char* s){
   char* new;
   size_t len = strlen(s) + 1;
   new = malloc(len);
   
   if(new)
      return (char*)memcpy(new,s,len);
   return NULL;
}

char* my_getpass(){
   struct termios old, new;
   char pass[256];

   //turns off echo
   tcgetattr(STDIN_FILENO,&old);

   new = old;
   new.c_lflag = new.c_lflag ^ ECHO;

   tcsetattr(STDIN_FILENO,TCSANOW,&new);

   //read in password
   printf("Enter your password: ");
   fgets(pass,256,stdin);

   //restore terminal defualt
   tcsetattr(STDIN_FILENO,TCSANOW,&old);

   return my_strdup(pass);
}

char* my_getuser(){
	char user[256];
	
	// read in username
	printf("Enter your username: ");
	fgets(user,255,stdin);
	
	return my_strdup(user);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void shutDown(int sockfd){
    int n;
    char buffer[256];
    char accesscode[7] = "SHUTDN";

    n = write(sockfd, accesscode, strlen(accesscode));
    if (n < 0) error ("ERROR writing to socket");
   
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) error ("ERROR reading from socket");
    printf("%s",buffer);

    close(sockfd);
}
struct sockaddr_in serverAdd(int argc, char *argv[]){
	int portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	Program = argv[0];
	if (argc < 3){
		fprintf(stderr, "userage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	server = gethostbyname(argv[1]);
	
	if (server == NULL){
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr_list[0],
		  (char *)&serv_addr.sin_addr.s_addr,
		  server->h_length);
	serv_addr.sin_port = htons(portno);

	return serv_addr;
}

int dataSocket(int argc, char *argv[])
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

	return sockfd;
}

/* Send game request from current client to another available user*/
/* Obtain available users from the server */
int sendGameRequests(int sockfd, int argc, char* argv[], char username[256])
{
	int n;
    char buffer[256];
    char list[1024];
	char opponent[256];

    char accesscode[7] = "SENDR";
    n = write(sockfd, accesscode, strlen(accesscode));
    if (n < 0) error ("ERROR writing to socket");
   
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) error ("ERROR reading from socket");
    
	n = write(sockfd, username, strlen(username));
	if (n < 0) error("ERROR writing to socket");

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
	if (n < 0) error("ERROR reading from socket");
	
	n = write(sockfd, "Requesting list of available players\n", 255);
	if (n < 0) error("ERROR writing to socket");

	bzero(list, 1024);
	n = read(sockfd, list, 1024);

    if(strcmp(list,"No players available.\n") == 0){
      printf("No players available.\n");
      return false; // no player is online
    }

	else{
    printf("\nHere is the list of available players:\n%s\n", list);

    printf("\nPlease enter the username of who you want to play with: ");

    bzero(opponent, 256);
    fgets(opponent,255,stdin);

	strcat(buffer, opponent); 
   }
	return true; // another player is online 
}

/* Process login for client */
char* login(int sockfd, int argc, char* argv[])
{
   int n, x,client;
   struct hostent *clientHost;
   char* pass;
   char* user;
   char buffer[256];
   char password[256];
   char username[256];
   char IPaddress[256];
   char hostBuf[256];
   char login[7] = "LOGIN";

	/* Store IP address in the address string*/
	char *address;
    client = gethostname(hostBuf,sizeof(hostBuf));
    clientHost = gethostbyname(hostBuf);
	address = inet_ntoa(*((struct in_addr*)clientHost->h_addr_list[0]));
	printf("Current client's IP address is: %s\n", address);
	strcpy(IPaddress, address); // IPaddress is used to write to socket
	char newline = '\n';
	strncat(IPaddress, &newline, 1); // New line char

    n = write(sockfd,login, strlen(login));
	if (n < 0) error("ERROR writing to socket");

	bzero(buffer, 256);
	n = read(sockfd, buffer, 255); /* Receive confirmation from server that IP address was received */
	if (n < 0) error("ERROR reading from socket");
	printf("%s", buffer);
 
	/* Write IP address to the socket for server to store */
	n = write(sockfd, IPaddress, strlen(IPaddress));
	if (n < 0) error("ERROR writing to socket");

	bzero(buffer, 256);
	n = read(sockfd, buffer, 255); /* Receive confirmation from server that IP address was received */
	if (n < 0) error("ERROR reading from socket");
	printf("%s", buffer);

	/* Send a valid username to server */
	do{
		bzero(buffer, 256);
		bzero(username, 256);
		user = my_getuser();
		strncpy(username, user, 255);
		free(user);
		n = write(sockfd, username, strlen(username)); /* Username is stored here */

		if (n < 0) error ("ERROR writing to socket");
		bzero(buffer, 256);

		n = read(sockfd, buffer, 255);

		if (n < 0) error("ERROR reading from socket");
		printf("%s\n", buffer);
	}while (strcmp(buffer, "\nAddress has already been registered.\n") == 0);

    bzero(buffer,256);
    n = read(sockfd,buffer,255); /* Receive confirmation from server that username exists or new account must be made */
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s",buffer);
    
	const char* errorMsg = "\n\nThat password was incorrect.\n";

	/* Send valid password to server */
	do{
    bzero(password,256);
    pass =  my_getpass();
    strncpy(password,pass,255);
    free(pass);
    x = write(sockfd,password,strlen(password));
    
    if (x < 0) 
         error("ERROR writing to socket");
    bzero(password,256);
    x = read(sockfd,password,255);
    if (x < 0) 
         error("ERROR reading from socket");
    printf("%s\n",password);
	} while(strcmp(password, errorMsg) == 0);


    return my_strdup(username);
}

int clientExit(int sockfd, int argc, char* argv[], char username[256])
{
    int n;
	char SendBuf[256];
    char buffer[256];

 	char accesscode[7] = "EXITS";

	n = write(sockfd, accesscode, strlen(accesscode)); /* Writes EXIT to the socket */
	if (n < 0) error("ERROR writing EXITS to socket");

	bzero(buffer, 256);
	n = read(sockfd, buffer, 255); /* Reads status message from socket */
	if (n < 0) error("ERROR reading status message from socket");

	strncpy(SendBuf, username, sizeof(username)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
	int l = strlen(SendBuf);
	n = write(sockfd, SendBuf, l); /* Sends username to socket */
	if (n < 0) error("ERROR writing username to socket");
	
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255); /* Reads status message from socket */
	if (n < 0) error("ERROR reading second status message from socket");

	return 0;
}
