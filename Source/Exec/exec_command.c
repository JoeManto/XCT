#include "exec_command.h"
#include "../xct.h"

extern Arguments* PRO_ARGS;

int validateCommand(void);

void setBuildOption(char* opt) {
    if (PRO_ARGS->noBuild) {
        strcpy(opt, "test-without-building");
    }
    else {
        strcpy(opt, "build-for-testing");
    }
}

int buildCommand(char* commandBuffer) {
    int result;
    if ((result = validateCommand()) != 0) {
        ulog(warning, "Command failed to validate");
        return result;
    }
    
    char build[128];
    setBuildOption(build);

    sprintf(
        commandBuffer,
        "xcodebuild test -scheme %s -only-testing %s",
        PRO_ARGS->scheme, PRO_ARGS->testTargetFile
    );

    return 0;
}

int validateCommand(void) {
    return 0;
}
