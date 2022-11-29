#include "../xct.h"

void exitOnError(char* err, int uxerr) {
    if (uxerr <= 0) {
        errno = uxerr;
    }
    perror(err);
    exit(1);
}

void continueOnWarning(char* warn) {
    printf("[WARNING] %s\n",warn);
}

// Expects value to be not be NULL
// free and nullifies the given ptr
void freeStr(char* ptr) {
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    }
}
