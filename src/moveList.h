#ifndef MOVELIST_H
#define MOVELIST_H

#include "Piece.h"
#include "Move.h"


   typedef struct ListNode{
      MOVE* move;
      struct ListNode* next;
   }ListNode;
   
   //create a node in the linked list to be stored
   ListNode* makeNode(MOVE* inputMove);

   //add node to the end of the list
   void addNode(MOVE* inputMove);

   //counts the number of nodes in a movelist
   int nodeCount(struct ListNode* node);   

   //returns first node in list
   ListNode* getHead();

   //return last node in list
   ListNode* getTail();

   //delete list
   void removeList();

   //print list of moves to the file
   void printToFile();

   
#endif 
