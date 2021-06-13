/* Player.c file : C file for chess player */
#include "Player.h"

/* Creates PLAYER* */
PLAYER* createPlayer(char c, int n){
    PLAYER *p;
    p = malloc(sizeof(PLAYER));

    if (!p)
        return NULL;

    p->color = c;
    p->num = n;

    return p;
}

/* Deletes PLAYER *p */
void deletePlayer(PLAYER* p){
    if (p != NULL)
        free(p);
}

/* Prints the information stored in PLAYER* p such as 'w1' */
void printPlayer(PLAYER* p){
    printf("%c%d\n", p->color, p->num);
}

/* Gets Player *p's color attribute as 'w' or 'b' */
char getPlayerColor(PLAYER* p){
    return p->color;
}

/* Gets Player *p's number attribute as an int 1 or 2 */
int getPlayerNum(PLAYER* p){
    return p->num;
}

/* Flips a player's color */
void changePlayerColor(PLAYER* p){
    assert(p);
    if (p->color == 'w')
        p->color = 'b';
    else
        p->color = 'w';
}

/* Flips a player's number */
void changePlayerNumber(PLAYER* p){
    assert(p);
    if (p->num == 1)
        p->num = 2;
    else
        p-> num = 1;
}
/* EOF*/

