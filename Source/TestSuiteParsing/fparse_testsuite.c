#include "fparse_testsuite.h"
#include "../TestCaseMatching/matcher.h"
#include <regex.h>

void fparse_prepare(void);
void fparse_restore(void);
uint8_t fparse_process(char* filePath);
uint8_t fparse_writeModifiedBuffer(void);
uint fparse_commentOutUnMatchedTestCase(char* buffer);
uint fparse_travelThroughTestCase(char* buffer);
uint64_t fparse_openTargetFile(char* filePath);
char* fparse_getCopyTargetFilePath(char* filePath);
void fparse_readFileIgnoringComments(void);
void fparse_searchTargetBuffer(char* buffer);
void fparse_addChar(char c);
void fparse_removeBlockComment(void);
void fparse_removeSingleComment(void);

extern Arguments* PRO_ARGS;
FILE* tar_file_ptr;

char* tar_file_buffer = NULL;
int tar_byteOffset = 0;
int tar_buffer_size = -1;

char* target_file_path;
char* target_file_copy_path;

void fparse_init(void) {
    matcher_init();
}

void fparse_dealloc(void) {
    matcher_dealloc();
    
    if (tar_file_ptr) {
        fclose(tar_file_ptr);
        tar_file_ptr = NULL;
    }
    
    if (tar_file_buffer) {
        free(tar_file_buffer);
        tar_file_buffer = NULL;
    }

    if (target_file_path) {
        free(target_file_path);
        target_file_path = NULL;
    }

    if (target_file_copy_path) {
        free(target_file_copy_path);
        target_file_copy_path = NULL;
    }
}

uint8_t fparse_start(void) {
    if (!PRO_ARGS->testTargetFile) {
        exitOnError("Expected target file", errno);
    }

    fparse_prepare();

    if (!fparse_process(target_file_path)) {
        // At this point we haven't touched the source file, so just remove the copy.
        remove(target_file_copy_path);
        exitOnError("Failed to process test target", errno);
    }

    if (!fparse_writeModifiedBuffer()) {
        ulog(error, "Failed to write to test target");
        return 1;
    }

    return 0;
}

void fparse_prepare(void) {
    // Find the actual system path of the provided test target file.
    target_file_path = malloc(sizeof(char) * (FILENAME_MAX + 1));
    memset(target_file_path, 0, FILENAME_MAX + 1);
    if (!util_findFile(".", target_file_path, PRO_ARGS->testTargetFile)) {
        exitOnError("Failed to find input file", errno);
    }

    if (*target_file_path == '\0') {
        exitOnError("Empty input file", errno);
    }

    ulogFormat(info, FILENAME_MAX, "Found file %s", target_file_path);

    if (!(target_file_copy_path = fparse_getCopyTargetFilePath(target_file_path))) {
        exitOnError("Failed to resolve copy path for test target", errno);
    }

    if (!util_copyFile(target_file_copy_path, target_file_path)) {
        exitOnError("Failed to copy test target", errno);
    }
}

void fparse_restore(void) {
    if (tar_file_ptr) {
        fclose(tar_file_ptr);
    }

    if (!util_copyFile(target_file_path, target_file_copy_path)) {
        // Don't remove the copy.
        exitOnError("Failed to replace test target with previous copy", errno);
    }

    if (remove(target_file_copy_path)) {
        exitOnError("Failed to remove test target copy", errno);
    }

    ulog(info, "Removed test target copy");
}

/// Attempts to open a given file and assigns tar_file_ptr on success.
/// Returns the size of the opened file.
uint64_t fparse_openTargetFile(char* filePath) {
    if ((tar_file_ptr = fopen(filePath, "r+")) == NULL) {
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

char* fparse_getCopyTargetFilePath(char* filePath) {
    // Create a copy of the file at the same path (.../copy-<file>)
    char* lastComponent = strchr(filePath, '/');
    char* fileCopyPath = malloc(sizeof(char) * (FILENAME_MAX + 1));
    memset(fileCopyPath, 0, FILENAME_MAX + 1);

    uint idx = 0;
    while (&filePath[idx] != lastComponent) {
        fileCopyPath[idx] = filePath[idx];
        idx++;
    }

    snprintf(fileCopyPath, FILENAME_MAX, "%s/%s-%s", fileCopyPath, "copy", PRO_ARGS->testTargetFile);
    return fileCopyPath;
}

// MARK: Buffer Writing

uint8_t fparse_writeModifiedBuffer(void) {
    if (tar_file_ptr) {
        fclose(tar_file_ptr);
    }

    if ((tar_file_ptr = fopen(target_file_path, "w+")) == NULL) {
        exitOnError("Couldn't open test target file", errno);
        return 0;
    }

    printf("Writing Buffer:\n%s\n", tar_file_buffer);
    if (fwrite(tar_file_buffer, tar_buffer_size, 1, tar_file_ptr) == 0) {
        ulog(error, "Failed to write buffer to test target");
        return 0;
    }

    return 1;
}

// MARK: Buffer Parsing

/// Starts the process of finding matching testcases in the given file
uint8_t fparse_process(char* filePath) {
    uint64_t fileLength = fparse_openTargetFile(filePath);
    if (fileLength == 0) {
        ulog(error, "Empty test target");
        return 0;
    }
    // Alloc file buffer
    tar_file_buffer = malloc(sizeof(char) * fileLength);

    // Read file into buffer
    fparse_readFileIgnoringComments();

    tar_buffer_size = tar_byteOffset;
    tar_byteOffset = 0;

    // Attempt to match testcases in buffer
    fparse_searchTargetBuffer(tar_file_buffer);

    printf("%s\n",tar_file_buffer);
    return 1;
}

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
uint fparse_travelThroughTestCase(char* buffer) {
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
uint fparse_commentOutUnMatchedTestCase(char* buffer) {
    uint curOffset = 0;

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
void fparse_readFileIgnoringComments(void) {
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
            fparse_removeBlockComment();
        }
        else if ( d == '/') {
            fparse_removeSingleComment();
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
void fparse_removeBlockComment(void) {
    char d;
    uint needsLinePadding = 0;
    uint isFirstCharacter = 1;

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
void fparse_removeSingleComment(void) {
    char d;
    while ((d=fgetc(tar_file_ptr)) != EOF) {
        if(d == '\n') {
            tar_file_buffer[tar_byteOffset++] = d;
            return;
        }
    }
}
