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
void env_setContext(char *path, Arguments* args);

Arguments* env_retrieveStoredArgs(void) {
    if (merged_args) {
        return merged_args;
    }

    env_setShortTermContext();
    env_setLongTermContext();

    merged_args = args_merge(long_term_args, short_term_args);

    return merged_args;
}

void env_setLongTermContext(void) {
    if (long_term_args) {
        return;
    }

    ulog(info, "Retrieving long term context");
    long_term_args = newArgs();
    env_setContext("/tmp/.xctrc-test-long", long_term_args);

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
    env_setContext("/tmp/.xctrc-test-short", short_term_args);

    if (short_term_args == NULL) {
        ulog(error, "Failed to retrieve short term context");
    }
}

void env_setContext(char *path, Arguments* args) {
    int result;
    if ((result = env_parseContextFile(path, args)) != 0) {
        exitOnError("Failed to parse context file", 0);
    }
}
