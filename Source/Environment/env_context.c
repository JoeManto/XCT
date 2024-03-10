//
//  env_context.c
//  XCT-Product
//
//  Created by Joe Manto on 8/7/23.
//

#include "env_context.h"
#include "env_context_parser.h"

Arguments* long_term_args;
Arguments* short_term_args;
Arguments* merged_args;

int env_getLongTermContext(Arguments** args);
int env_getShortTermContext(Arguments** args);
void env_setContext(char *path, Arguments** args);

Arguments* env_retrieveStoredArgs(void) {
    if (long_term_args == NULL) {
        ulog(info, "Retrieving long term context");
        if (env_getLongTermContext(&long_term_args) != 0) {
            ulog(error, "Failed to set long term args");
        }
    }

    if (short_term_args == NULL) {
        ulog(info, "Retrieving short term context");
        if (env_getShortTermContext(&short_term_args) != 0) {
            ulog(error, "Failed to set short term args");
        }
    }

    return long_term_args;
}

int env_getLongTermContext(Arguments** args) {
    *args = newArgs();
    env_setContext("/tmp/.xctrc-test-long", args);
    return 0;
}

int env_getShortTermContext(Arguments** args) {
    *args = newArgs();
    env_setContext("/tmp/.xctrc-test-short", args);
    return 0;
}

void env_setContext(char *path, Arguments** args) {
    int result;
    if ((result = env_parseContextFile(path, *args)) != 0) {
        exitOnError("Failed to parse context file", 0);
    }
}
