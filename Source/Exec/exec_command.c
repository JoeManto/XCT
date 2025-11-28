#include "exec_command.h"
#include "../xct.h"

extern Arguments* PRO_ARGS;

int _validateCommand(void);
void _setBuildOption(char* opt);

int buildCommand(char* commandBuffer) {
    int result;
    if ((result = _validateCommand()) != 0) {
        ulog(warning, "Command failed to validate");
        return result;
    }
    
    char build[128];
    _setBuildOption(build);

    sprintf(
        commandBuffer,
        "xcodebuild test -scheme %s -only-testing %s/%s",
        PRO_ARGS->scheme, PRO_ARGS->target, PRO_ARGS->testTargetFile
    );

    return 0;
}

void _setBuildOption(char* opt) {
    if (PRO_ARGS->noBuild) {
        strcpy(opt, "test-without-building");
    }
    else {
        strcpy(opt, "build-for-testing");
    }
}

int _validateCommand(void) {
    return 0;
}
