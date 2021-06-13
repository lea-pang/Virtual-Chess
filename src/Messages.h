#ifndef MESSAGES_H
#define MESSAGES_H

#define _BSD_SOURCE
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "onlineGame.h"

// should send current client's message to the server
int sendMessage(int sockfd);

/* specific chat log skeleton for choice #4 */
void msgMain(int sockfd, PLAYER* p1, PLAYER* p2);

/* viewing or receiving a message? */
char* receiveMessage(int sockfd);
#endif
