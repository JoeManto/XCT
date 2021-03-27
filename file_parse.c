#include "file_parse.h"
#include "xct.h"
#include "args.h"
#include "matcher.h"
#include <regex.h>

extern args* PRO_ARGS;
FILE* tar_file_ptr;
char * tar_file_buffer = NULL;
int tar_byteOffset = 0;
int tar_file_size = -1;

void fparse_init(){
  matcher_init();
}

void fparse_dealloc(){
  matcher_dealloc();
}

int fparse_start(char * fileName) {
  // Read Test target
  if ((tar_file_ptr = fopen(fileName, "r")) == NULL) {
    exitOnError("Couldn't open test target file", -1);
  }

  // set file offset
  fseek (tar_file_ptr, 0, SEEK_END);

  // Gather file size
  long file_length = ftell(tar_file_ptr);
  if (file_length == -1) {
    exitOnError("ftell error", EFBIG);
  }

  // Reset Offset Back
  // Alloc space for whole file in buffer
  fseek (tar_file_ptr, 0, SEEK_SET);
  tar_file_buffer = malloc(file_length);

  // Removes Comments but also loads file into buffer
  fparse_removeComments();
  tar_file_size = tar_byteOffset;
  tar_byteOffset = 0;

  fparse_parseBufferLineByLine(tar_file_buffer);

  fseek (tar_file_ptr, 0, SEEK_SET);
  printf("%s\n",tar_file_buffer);

  fclose(tar_file_ptr);
  return 0;
}

void fparse_parseBufferLineByLine(char* buffer){
  char * curLine = buffer;
  int offset = 1;

  while(curLine) {
    // Find the first newLine
    char * nextLine = strchr(curLine, '\n');

    // optimization, curLine is already pointing to a newLine
    if (nextLine == curLine) {
      curLine++;
      continue;
    }

    // Temporarily set a null term there so we can focus on characters in curline...nextline - 1 range
    if(nextLine) *nextLine = '\0';

    int testCasePadding = matcher_lineDoesContainTestHeader(curLine);

    // The current line doesn't contain a test case
    if (testCasePadding <= 0) {
      // No match found reset the newline to where it was
      if (nextLine) *nextLine = '\n';
      // Set curLine to the position of the first '\n' + 1
   
      curLine = nextLine ? (nextLine + offset) : NULL;
      continue;
    }

    // The current line does contain a test case check if its a valid test case
    int isMatch = matcher_testCaseDoesMatch(curLine, testCasePadding);

    // Reset the null term early so all lines of the testcase can be parsed
    if (nextLine) *nextLine = '\n';

    if (isMatch) {
      if ((offset = fparse_travelThroughTestCase(curLine)) == -1) {
        exitOnError("Compilation Error - expected closing '}' token", EINVAL);
      }
      
      while (curLine[offset] && curLine[offset] != '\n') {
        offset++;
      }
    }
    else {
      offset = fparse_commentOutUnMatchedTestCase(curLine);
    }

    curLine = curLine + offset;
    // Reset the offset to normal
    offset = 1;
  }
}

int fparse_travelThroughTestCase(char * buffer) {
  // Determines nested block clauses
  int depth = 0;
  int curOffset = 0;

  // Find function starting block clause
  for (;curOffset <= tar_file_size; curOffset++) {
    if (buffer[curOffset] == '{') {
      depth = 1;
      // Increase the offset so we don't double count '{'
      curOffset++;
      break;
    }
  }

  // Find function ending block clause
  // add ending part of block comment
  for (;curOffset <= tar_file_size; curOffset++) {
    if (depth == 0) {
      return curOffset;
    }
    if (buffer[curOffset] == '{') { depth++; }
    if (buffer[curOffset] == '}') { depth--; }
  }

  // Reached EOF with uneven depth
  return -1;
}

int fparse_commentOutUnMatchedTestCase(char * buffer) {
  int curOffset = 0;

  // Start block comment
  buffer[0] = '/';
  buffer[1] = '*';

  if ((curOffset = fparse_travelThroughTestCase(buffer)) == -1) {
    exitOnError("Compilation Error - expected closing '}' token", EINVAL);
  }

  buffer[curOffset - 2] = '*';
  buffer[curOffset - 1] = '/';

  return curOffset;
}

//******************************************************************************
//                            Remove Comments
//******************************************************************************

void fparse_removeComments() {
  char c;
  while ((c=fgetc(tar_file_ptr))!=EOF) {
    fparse_lookForComment(c);
  }
}

// Handles both types of comments
void fparse_lookForComment(char c) {
  char d;
  if (c == '/') {
    if ((d=fgetc(tar_file_ptr))=='*') {
      fparse_handleBlockComment();
    } else if ( d == '/') {
      fparse_handleSingleComment();
    } else {
      tar_file_buffer[tar_byteOffset++] = c;
      tar_file_buffer[tar_byteOffset++] = d;
    }
  } else {
    tar_file_buffer[tar_byteOffset++] = c;
  }
}

// Handles block comments
// Increments file curser until block comment is closed.
void fparse_handleBlockComment() {
  char d;
  int needsLinePadding = 0;

  while ((d=fgetc(tar_file_ptr)) != EOF) {
    if (d == '\n') needsLinePadding = 1;

    if (d == '/' && (d=fgetc(tar_file_ptr)) == '*') {
      exitOnError("Parsing Error - nested block comment found", EINVAL);
    }

    if (d != '*') continue;

    // Handles mutiple '*'s in comment ending
    while (d == '*') d = fgetc(tar_file_ptr);
    if (d == '/') break;
  }

  // If the block comment spans mutiple lines, a new line buffer is needed
  // to ensure the line at the end of the block is not combined with block start.
  if (needsLinePadding) {
    tar_file_buffer[tar_byteOffset++] = '\n';
  }
}

// Handles single line comments
// Increments file curser until new line.
void fparse_handleSingleComment() {
  char d;
  while ((d=fgetc(tar_file_ptr)) != EOF) {
    if(d == '\n') {
      tar_file_buffer[tar_byteOffset++] = d;
      return;
    }
  }
}
