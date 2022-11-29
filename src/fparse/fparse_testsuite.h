#ifndef FILE_PARSE_DOT_H
#define FILE_PARSE_DOT_H

#include "../xct.h"

void fparse_init();
void fparse_dealloc();
void fparse_searchTargetBuffer(char* buffer);
int fparse_process(char* fileName);
int fparse_commentOutUnMatchedTestCase(char* buffer);
int fparse_travelThroughTestCase(char* buffer);
long fparse_openTargetFile(char* fileName);

void fparse_readFileIgnoringComments();
void fparse_addChar(char c);
void fparse_removeBlockComment();
void fparse_removeSingleComment();

#endif /* TEST_FILE_PARSER_DOT_H */
