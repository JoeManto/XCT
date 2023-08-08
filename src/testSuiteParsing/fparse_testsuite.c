#include "fparse_testsuite.h"
#include "../args/args.h"
#include "../testCaseMatching/matcher.h"
#include <regex.h>

extern args* PRO_ARGS;
FILE* tar_file_ptr;

char* tar_file_buffer = NULL;
int tar_byteOffset = 0;
int tar_buffer_size = -1;

void fparse_init() {
    matcher_init();
}

void fparse_dealloc() {
    matcher_dealloc();
    
    if (tar_file_ptr) {
        fclose(tar_file_ptr);
        tar_file_ptr = NULL;
    }
    
    if (tar_file_buffer) {
        free(tar_file_buffer);
        tar_file_buffer = NULL;
    }
}

/// Starts the process of finding matching testcases in the given file
int fparse_process(char* fileName) {
    long fileLength = fparse_openTargetFile(fileName);
    
    // Alloc file buffer
    tar_file_buffer = malloc(sizeof(char) * fileLength);
    
    // Read file into buffer
    fparse_readFileIgnoringComments(tar_file_ptr);
    
    tar_buffer_size = tar_byteOffset;
    tar_byteOffset = 0;
    
    // Attempt to match testcases in buffer
    fparse_searchTargetBuffer(tar_file_buffer);
    
    printf("%s\n",tar_file_buffer);
    fclose(tar_file_ptr);
    return 0;
}

/// Attempts to open a given file and assigns tar_file_ptr on success.
/// Returns the size of the opened file.
long fparse_openTargetFile(char* fileName) {
    if ((tar_file_ptr = fopen(fileName, "r")) == NULL) {
        exitOnError("Couldn't open test target file", -1);
        return -1;
    }
    
    // Get file size
    fseek(tar_file_ptr, 0, SEEK_END);
    long file_length = ftell(tar_file_ptr);
    fseek(tar_file_ptr, 0, SEEK_SET);
    
    if (file_length == -1) {
        exitOnError("ftell error", EFBIG);
    }
    
    return file_length;
}

// MARK: Buffer Parsing

/// Iterates through target file buffer line by line checking for testcases that match the matching requirements
/// Testcases that don't match are commented out in the buffer
void fparse_searchTargetBuffer(char* buffer) {
    char* curLine = buffer;
    int offset = 1;
    
    while(curLine) {
        // Find the first newLine
        char* nextLine = strchr(curLine, '\n');
        
        // Optimization, curLine is already pointing to a newLine
        if (nextLine == curLine) {
            curLine++;
            continue;
        }
        
        // Temporarily limit buffer stream to just characters in range [curline...nextline - 1]
        if (nextLine) *nextLine = '\0';
        
        int testCasePadding = matcher_containsTestHeader(curLine);
        
        // The current line doesn't contain a test case
        if (testCasePadding < 0) {
            // Remove stream limit
            if (nextLine) *nextLine = '\n';
            
            // Set curLine to the position of the first '\n' + 1
            curLine = nextLine ? (nextLine + offset) : NULL;
            continue;
        }
        
        int isMatch = matcher_doesTestCaseMatch(curLine, testCasePadding);
        
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

/// Travels through a non matching testcase function
/// Returns the new index in the buffer to continue scanning. If the end of the file is reached -1 is returned
int fparse_travelThroughTestCase(char* buffer) {
    int depth = 0;
    int curOffset = 0;
    
    // Find function starting block clause
    for (;curOffset <= tar_buffer_size; curOffset++) {
        if (buffer[curOffset] == '{') {
            depth = 1;
            // Increase the offset so we don't double count '{'
            curOffset++;
            break;
        }
    }
    
    // Find function ending block clause
    // add ending part of block comment
    for (;curOffset <= tar_buffer_size; curOffset++) {
        if (depth == 0) {
            return curOffset;
        }
        if (buffer[curOffset] == '{') { depth++; }
        if (buffer[curOffset] == '}') { depth--; }
    }
    
    // Reached EOF with uneven depth
    return -1;
}

/// Travels through a non matching testcase function. Adding block comment components at the start and end of the testcase
/// Returns the new index in the buffer to continue scanning. If the end of the file is reached -1 is returned
int fparse_commentOutUnMatchedTestCase(char* buffer) {
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

// MARK: Buffer Loading

/// Loads file into buffer while skipping over comments
void fparse_readFileIgnoringComments() {
    char c;
    while ((c=fgetc(tar_file_ptr)) != EOF) {
        fparse_addChar(c);
    }
}

/// Adds given file character to buffer. If the character indicates the start of a comment.
/// The file curser will be increased until the comment is ended
void fparse_addChar(char c) {
    char d;
    if (c == '/') {
        if ((d=fgetc(tar_file_ptr)) == '*') {
            fparse_removeBlockComment(tar_file_ptr);
        }
        else if ( d == '/') {
            fparse_removeSingleComment(tar_file_ptr);
        }
        else {
            tar_file_buffer[tar_byteOffset++] = c;
            tar_file_buffer[tar_byteOffset++] = d;
        }
    }
    else {
        tar_file_buffer[tar_byteOffset++] = c;
    }
}

/// Removes block comments
/// Increments file curser until block comment is closed
void fparse_removeBlockComment() {
    char d;
    int needsLinePadding = 0;
    int isFirstCharacter = 1;
    
    while ((d=fgetc(tar_file_ptr)) != EOF) {
        // Handle case where same '*' is used for the start and end of block comment
        if (isFirstCharacter && d == '/') {
            break;
        }
        isFirstCharacter = 0;
        
        if (d == '\n') needsLinePadding = 1;
        
        if (d == '/' && (d=fgetc(tar_file_ptr)) == '*') {
            exitOnError("Parsing Error - Nested block comment found", EINVAL);
        }
        
        if (d != '*') {
            continue;
        }
        
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

/// Removes single line comments
/// Increments file curser until new line
void fparse_removeSingleComment() {
    char d;
    while ((d=fgetc(tar_file_ptr)) != EOF) {
        if(d == '\n') {
            tar_file_buffer[tar_byteOffset++] = d;
            return;
        }
    }
}
