#ifndef MATCHER_DOT_H
#define MATCHER_DOT_H

typedef struct Match {
    char * matchedLine;
    int testOffset;
    int testLength;
    int valid;
} match;

void matcher_init();
void matcher_dealloc();
int matcher_match(char * string, char * pattern, int * foundAtIndex);
int matcher_containsTestHeader(char* line);
int matcher_doesTestCaseMatch(char * testCase, int testCasePadding);
void matcher_appendMatch(char * line,int line_start, int line_end);
#endif /* MATCHER_PARSER_DOT_H */
