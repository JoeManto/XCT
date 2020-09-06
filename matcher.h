#ifndef MATCHER_DOT_H
#define MATCHER_DOT_H

typedef struct Matches {
    char * matchedLine;
    int testOffset;
    int testLength;
    int valid;
} match;

void matcher_init();
void matcher_dealloc();
int matcher_lineDoesContainTestHeader(char* line);
int matcher_Comment(char* line);
void matcher_appendMatch(char * line,int line_start, int line_end);
#endif /* MATCHER_PARSER_DOT_H */
