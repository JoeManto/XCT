#include "xct.h"

void exitOnError(char * err, int uxerr){
  if(errno != -1){
    errno = uxerr;
  }
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
