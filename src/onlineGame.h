#ifndef ONLINEGAME_H
#define ONLINEGAME_H
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
#include "humanGame.h"
#include "client.h"
#include "util.h"

void Human_v_Human_Online(int sockfd, int argc, char *argv[], PLAYER *p1, PLAYER *p2, BOARD *b);

int sendMove(int sockfd, int argc, char *argv[], char move[5]);

char* receiveMove(int sockfd, int argc, char *argv[]);

#endif
