#ifndef UTIL_DOT_H
#define UTIL_DOT_H

enum LogMessage {
    info = 0,
    warning,
    error,
} typedef LogMessage;

void exitOnError(char* err, uint uxerr);
void ulog(LogMessage level, char *message);
void ulogFormat(LogMessage level, uint8_t size, char *message, const char * format, ... );

#endif /* UTIL_DOT_H */
