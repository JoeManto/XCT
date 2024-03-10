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
    unsigned long size = strlen(src);
    unsigned long ignoreSetSize = strlen(ignoreSet);

    int dstIndex = 0;
    for (int i = 0; i < size; i++) {
        char character = src[i];

        bool ignore = false;
        for (int x = 0; x < ignoreSetSize; x++) {
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
    unsigned long size = strlen(str);

    for (int i = 0; i < size; i++) {
        str[i] = tolower(str[i]);
    }
}
