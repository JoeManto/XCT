#ifndef MATCHER_DOT_H
#define MATCHER_DOT_H

#include <inttypes.h>

typedef struct Match {
    char * matchedLine;
    uint testOffset;
    uint testLength;
    uint valid;
} match;

void matcher_init(void);
void matcher_dealloc(void);
int matcher_match(char* string, char* pattern, int* foundAtIndex);
int matcher_containsTestHeader(char* line);
int matcher_doesTestCaseMatch(char* testCase, int testCasePadding);
void matcher_appendMatch(char* line, uint line_start, uint line_end);
#endif /* MATCHER_PARSER_DOT_H */
