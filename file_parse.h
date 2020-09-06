#ifndef FILE_PARSE_DOT_H
#define FILE_PARSE_DOT_H

void fparse_parseBufferLineByLine(char* buffer);
int fparse_start(char * fileName);
int fparse_commentOutUnMatchedTestCase(char * buffer);
int fparse_travelThroughTestCase(char * buffer);

void fparse_removeComments();
void fparse_lookForComment(char c);
void fparse_handleBlockComment();
void fparse_handleSingleComment();

#endif /* TEST_FILE_PARSER_DOT_H */
