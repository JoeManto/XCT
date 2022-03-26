#include "matcher.h"
#include "../xct.h"
#include "../args/args.h"
#include <regex.h>

#define DEFAULT_MATCH_SIZE 10
#define TESTCASE_PATTERN "\w*func test"

extern args* PRO_ARGS;

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

int matcher_match(char * string, char * pattern, int * foundAtIndex) {
  regex_t    preg;
	int        rc;
	size_t     nmatch = 1;
	regmatch_t pmatch[2];

	if (0 != (rc = regcomp(&preg, pattern, REG_EXTENDED))) {
		 continueOnWarning("Failed to compile regex expression. Parsing could be effected");
     return rc;
	}

	if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
		 //printf("Failed to match '%s' with '%s',returning %d.\n",
		//				string, pattern, rc);
	} else {
    *foundAtIndex = pmatch[0].rm_eo - pmatch[0].rm_so;
    return 1;
	}
	regfree(&preg);
  return rc;
}

// Determines if a given line contains a XC TestCase
// @returns
// Failure: < 0
// Success: The index in the curline in which the pattern found a match
int matcher_lineDoesContainTestHeader(char* line) {
  char* pattern = "\w*func test";
  int testCasePadding = -1;
  int isMatch = matcher_match(line, pattern, &testCasePadding);

  if(testCasePadding != -1){
    //printf("%s %d\n", "match found at", testCasePadding);
  }

  return testCasePadding;
}

char * trimTestCase(char * testCase, int testCasePadding) {
  printf("testCaseBefore:%s\n", testCase);
  int offset = testCasePadding + strlen("test");
  testCase += offset;

  offset = 0;
  for (;testCase[offset] != '('; offset++) {
    if (testCase[offset] == EOF) {
      break;
    }
  }
  testCase[offset] = '\0';
  printf("testCaseAfter:%s\n", testCase);
  return testCase;
}

int matcher_regex(char* testCase) {
  printf("SDFSFDSDF");
  return 1;
}

int matcher_substring(char* testCase) {
  strstr(testCase, PRO_ARGS->matchingString);
  return 1;
}

int matcher_exact(char* testCase) {
  if (!PRO_ARGS->matchingString) {
    return 0;
  }
  return strcmp(testCase, PRO_ARGS->matchingString) == 0;
}

int matcher_testCaseDoesMatch(char * testCase, int testCasePadding) {
  int isMatch = 0;

  testCase = trimTestCase(testCase, testCasePadding);
  //printf("Matcher = %d\n", PRO_ARGS->matchingType);
  switch(PRO_ARGS->matchingType){
    case regex:
      isMatch = matcher_regex(testCase);
      break;
    case substring:
      isMatch = matcher_substring(testCase);
      break;
    case exact:
      isMatch = matcher_exact(testCase);
      break;
  }
  printf("testcase:%s\n", testCase);
  printf("matching type:%i\n", PRO_ARGS->matchingType);
  printf("matching:%s\n", PRO_ARGS->matchingString);
  printf("%s\n", isMatch ? "Match" : "No Match");
  return isMatch;
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
