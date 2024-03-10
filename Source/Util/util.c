#include "../xct.h"

void exitOnError(char* err, int uxerr) {
    if (uxerr <= 0) {
        errno = uxerr;
    }
    perror(err);
    exit(1);
}

void ulog(LogMessage level, char *message) {
    switch (level) {
        case info:
            printf("[INFO] %s\n", message);
            break;
        case warning:
            printf("[WARNING] %s\n", message);
            break;
        case error:
            printf("[ERROR] %s\n", message);
            break;
        default:
            break;
    }
}
