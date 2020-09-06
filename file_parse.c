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

int fparse_start(char * fileName) {
  if((tar_file_ptr = fopen(fileName, "r")) == NULL){
    exitOnError("Couldn't open test target file");
  }

  long length;

  // set file offset
  fseek (tar_file_ptr, 0, SEEK_END);
  // Gather file size
  length = ftell(tar_file_ptr);
  if(length == -1){
    exitOnError("ftell error");
  }

  // Reset Offset Back
  // Alloc space for whole file in buffer
  fseek (tar_file_ptr, 0, SEEK_SET);
  tar_file_buffer = malloc(length);

  // Removes Comments but also loads file into buffer
  fparse_removeComments();
  tar_file_size = tar_byteOffset;
  tar_byteOffset = 0;

  //fparse_parseBufferLineByLine(tar_file_buffer);

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
    // Temporarily set a null term there
    if(nextLine) *nextLine = '\0';

    // The current line doesnt contain a test case
    if(!matcher_lineDoesContainTestHeader(curLine)) {
      // No match found reset the newline to where it was
      if(nextLine) *nextLine = '\n';
      curLine = nextLine ? (nextLine+offset) : NULL;
      continue;
    }

    // Reset the null term early so all lines of the testcase can be parsed
    if(nextLine) *nextLine = '\n';

    if(0/*test case matches*/){
      if((offset = fparse_travelThroughTestCase(curLine)) == -1){
        exitOnError("Compilation Error - expected closing '}' token");
      }
    }else{
      fparse_commentOutUnMatchedTestCase(curLine);
    }

    // Increment the buffer pointer to the start of (nextLine + 1)
    // Offset is defualt 1
    curLine = nextLine ? (nextLine+offset) : NULL;
  }
}

int fparse_travelThroughTestCase(char * buffer) {
  // Determines nested block clauses
  // 0 mean none
  int depth = 0;
  int curOffset = 0;

  // Find function starting block clause
  for(;curOffset <= tar_file_size; curOffset++) {
    if(buffer[curOffset] == '{'){
      depth = 1;
      break;
    }
  }

  // Find function ending block clause
  // add ending part of block comment
  for(;curOffset <= tar_file_size; curOffset++){
    if(depth == 0) {
      return curOffset + 1;
    }
    if(buffer[curOffset] == '{') depth++;
    if(buffer[curOffset] == '}') depth--;
  }

  // Reached EOF with uneven depth
  return -1;
}

int fparse_commentOutUnMatchedTestCase(char * buffer) {
  int curOffset = 0;

  // Start block comment
  buffer[0] = '/';
  buffer[1] = '*';

  if((curOffset = fparse_travelThroughTestCase(buffer)) == -1){
    exitOnError("Compilation Error - expected closing '}' token");
  }

  buffer[curOffset - 2] = '*';
  buffer[curOffset - 1] = '/';

  /*
  // Determines nested block clauses
  // 0 mean none
  int depth = 0;

  // Find function starting block clause
  for(;curOffset <= tar_file_size; curOffset++) {
    if(buffer[curOffset] == '{'){
      depth = 1;
      break;
    }
  }

  // Find function ending block clause
  // add ending part of block comment
  for(;curOffset <= tar_file_size; curOffset++){
    if(depth == 0) {
      buffer[curOffset - 1] = '*';
      buffer[curOffset] = '/';
      return curOffset + 1;
    }
    if(buffer[curOffset] == '{') depth++;
    if(buffer[curOffset] == '}') depth--;
  }*/

  return curOffset;
}

//******************************************************************************
//                            Remove Comments
//******************************************************************************

void fparse_removeComments(){
  char c;
  while((c=fgetc(tar_file_ptr))!=EOF){
    fparse_lookForComment(c);
  }
}

// Handles both types of comments
void fparse_lookForComment(char c) {
  char d;
  if(c == '/') {
    if((d=fgetc(tar_file_ptr))=='*') {
      fparse_handleBlockComment();
    } else if( d == '/') {
      fparse_handleSingleComment();
        } else {
          tar_file_buffer[tar_byteOffset++] = c;
          tar_file_buffer[tar_byteOffset++] = d;
        }
  } else {
    tar_file_buffer[tar_byteOffset++] = c;
  }
}


// Function that handles block comments
// Increments offset until block comment is finished.
void fparse_handleBlockComment(){
  char d,e;
  while((d=fgetc(tar_file_ptr)) != EOF && (e=fgetc(tar_file_ptr)) != EOF) {
    if(d == '*' && e == '/') {
      return;
    }
  }
}

// Function that handles single line comments
// Increments offset until line comment is finished.
void fparse_handleSingleComment(){
  char d,e;
  while(((d=fgetc(tar_file_ptr)) != EOF) && d != '\n');
}
