#include "../xct.h"

void exitOnError(char* err, uint uxerr) {
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

void ulogFormat(LogMessage level, uint8_t size, char *message, const char * format, ... ) {
    char buffer[size];
    memset(buffer, 0, size);
    snprintf(buffer, size, message, format);
    ulog(level, buffer);
}
