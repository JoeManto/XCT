//
//  string+util.c
//  XCT-Product
//
//  Created by Joe Manto on 3/9/24.
//

#include "../xct.h"
#include "string+util.h"

// Expects value to be not be NULL
// free and nullifies the given ptr
void freeStr(char* ptr) {
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    }
}

void strcpy_ignore(char* dst, char* src, char* ignoreSet) {
    uint64_t size = strlen(src);
    uint64_t ignoreSetSize = strlen(ignoreSet);

    uint64_t dstIndex = 0;
    for (uint64_t i = 0; i < size; i++) {
        char character = src[i];

        bool ignore = false;
        for (uint64_t x = 0; x < ignoreSetSize; x++) {
            if (character == ignoreSet[x]) {
                ignore = true;
                break;
            }
        }

        if (ignore) {
            continue;
        }

        dst[dstIndex++] = src[i];
    }

    for (; dstIndex < size; dstIndex++) {
        dst[dstIndex] = '\0';
    }
}

void strlower(char* str) {
    uint64_t size = strlen(str);

    for (uint64_t i = 0; i < size; i++) {
        str[i] = tolower(str[i]);
    }
}

uint64_t strcount(char character, char* str) {
    size_t length = strlen(str);
    uint64_t count = 0;

    for (uint64_t i = 0; i < length; i++) {
        if (str[i] == character) {
            count++;
        }
    }

    return count;
}

uint8_t str_hasExtension(char* fileName, char* extension) {
    fileName = strrchr(fileName, '.');

    if (fileName != NULL) {
        return strcmp(fileName + sizeof(char), extension) == 0;
    }

    return 0;
}
