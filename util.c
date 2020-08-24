#include "xct.h"

void exitOnError(char * err){
  perror(err);
  exit(1);
}

// Expects value to be not be NULL
// free and nullifies the given ptr
void clearPropertyIfNeeded(char * ptr) {
  if(ptr != NULL){
    free(ptr);
    ptr = NULL;
  }
}
