#include "util.h"

char* strdup(const char* s){
   char* new;
   size_t len = strlen(s) + 2;
   new = malloc(len);
  
   if(new){
      return (char*)memcpy(new,s,len);
   }
   return NULL;
}
