#ifndef UTIL_DOT_H
#define UTIL_DOT_H

enum LogMessage {
    info = 0,
    warning = 1,
    error = 2,
} typedef LogMessage;

void exitOnError(char * err, int uxerr);
void ulog(LogMessage level, char *message);

#endif /* UTIL_DOT_H */
