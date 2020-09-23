#include "matcher.h"
#include "xct.h"
#include <regex.h>

#define DEFAULT_MATCH_SIZE 10

int num_matches;
match* MATCHES;

void matcher_init(){
  num_matches = 0;
  MATCHES = malloc(sizeof(struct Matches) * DEFAULT_MATCH_SIZE);
}

void matcher_dealloc(){
  free(MATCHES);
  MATCHES = NULL;
}

int matcher_lineDoesContainTestHeader(char* line){
  regex_t regex;
  int match = regexec(&regex, "func test", 0, NULL, 0);
  return match != REG_NOMATCH ? 0 : 1;
}

int matcher_Comment(char* line){
  /*regex_t regex;
  int isBlockComment = regexec(&regex, "(^[\/]?[\/])|(^[\/]?[*])|(^[*])");*/
  return 0;
}

void matcher_appendMatch(char * line,int line_start, int line_end){
  // check if more space is needed
  // If so extend by a 10 matches
  if(num_matches % 10 == 0){
    match* temp = realloc(MATCHES, sizeof(struct Matches) * (num_matches + DEFAULT_MATCH_SIZE));
    if(temp == NULL){
      exitOnError("Memory Error", ENOMEM);
    }else{
      MATCHES = temp;
    }
  }
  num_matches++;
}
