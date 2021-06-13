#ifndef CLIENT_H
#define CLIENT_H
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

char* my_strdup(const char* s);

char* my_getpass();

char* my_getuser();

void error(const char *msg);

struct sockaddr_in serverAdd(int argc, char *argv[]);

int dataSocket(int argc, char *argv[]);

void shutDown(int sockfd);

int sendGameRequests(int sockfd, int argc, char *argv[], char username[256]);

int checkGameRequests(int sockfd, int argc, char *argv[]);

char* login(int sockfd, int argc, char *argv[]);

int clientExit(int sockfd, int argc, char *argv[], char username[256]);

#endif
