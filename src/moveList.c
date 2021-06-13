#include "moveList.h"

   static ListNode* head = NULL;
   static ListNode* tail = NULL;

   //create a node in the linked list to be stored
   ListNode* makeNode(MOVE* inputMove){
      ListNode* newNode = malloc(sizeof(ListNode));
      newNode->move = inputMove;
      newNode->next = NULL;
      return newNode;
   }

   //add node to the end of the list
   void addNode(MOVE* inputMove){
      ListNode* newNode = makeNode(inputMove);

      if(head == NULL){
         head = tail = newNode;
         return;
      }

      tail->next = newNode;
      tail = newNode;
   }

   ListNode* getHead(){
      return head;
   }

   ListNode* getTail(){
      return tail;
   }

   //counts how many nodes are in the list
   int nodeCount(struct ListNode* head)
   {
	int count = 0;
	//initializes the node to the head
	struct ListNode* node = head;
	//adds a count while a node is not empty (implies it's the last one)
	while(node!=NULL)
	{
		count++;
		node = node->next;
	}
	return count;
   } 
   //delete list
   void removeList(){
      ListNode* node = getHead();
      ListNode* next;
      
      while(node != NULL){
         next = node->next;
         if(node->move != NULL){
            deleteMove(node->move);
         }
         free(node);
         node = next;
      }
	head = NULL;
	tail = NULL;
   }
                      
   //print list of moves to the file
   void printToFile(PLAYER* p1, PLAYER* p2){
      FILE* file;
      
      PLAYER* p;
      p = p1;
      ListNode* newHead = getHead();
      
      file = fopen("./bin/log.txt","w+");
      fprintf(file,"Log of player moves in the most recent game\n\n");
      
      while(newHead != NULL){
         fprintf(file,"Player %d (%c): %s%s\n",p->num,p->color,newHead->move->start, newHead->move->end);
         
         if(p->num == 1){
            p = p2;
         }
         
         else if(p->num == 2){
            p = p1;
         }
         
         newHead = newHead->next;
         }  

      fprintf(file,"\n#EOF");
      fclose(file);
      }
