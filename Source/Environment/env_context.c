//
//  env_context.c
//  XCT-Product
//
//  Created by Joe Manto on 8/7/23.
//

#include "env_context.h"
#include "env_context_parser.h"
#include "../Arguments/arg_set.h"

Arguments* long_term_args;
Arguments* short_term_args;
Arguments* merged_args;

void env_setLongTermContext(void);
void env_setShortTermContext(void);
void env_setContext(char* fileName, Arguments* args);

Arguments* env_retrieveStoredArgs(void) {
    if (merged_args) {
        return merged_args;
    }

    env_setShortTermContext();
    env_setLongTermContext();

    merged_args = args_merge_new(long_term_args, short_term_args);

    return merged_args;
}

void env_setLongTermContext(void) {
    if (long_term_args) {
        return;
    }

    ulog(info, "Retrieving long term context");
    long_term_args = newArgs();
    env_setContext(".xctrc-long", long_term_args);

    if (long_term_args == NULL) {
        ulog(error, "Failed to retrieve long term context");
    }
}

void env_setShortTermContext(void) {
    if (short_term_args) {
        return;
    }

    ulog(info, "Retrieving short term context");
    short_term_args = newArgs();
    env_setContext(".xctrc-short", short_term_args);

    if (short_term_args == NULL) {
        ulog(error, "Failed to retrieve short term context");
    }
}

char* env_contextFilePath(char* fileName) {
    char* homePathVar = getenv("HOME");
    char* filePath = malloc(sizeof(char) * (strlen(fileName) + strlen(homePathVar) + 2)); // +1 '/' and +1 for null-term
    strcat(filePath, homePathVar);
    strcat(filePath, "/");
    strcat(filePath, fileName);
    return filePath;
}

void env_setContext(char* fileName, Arguments* args) {
    char* filePath = env_contextFilePath(fileName);

    uint result;
    if ((result = env_parseContextFile(filePath, args)) != 0) {
        exitOnError("Failed to parse context file", 0);
    }

    free(filePath);
}
