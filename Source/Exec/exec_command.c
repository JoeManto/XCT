#include "exec_command.h"
#include "../xct.h"

extern args* PRO_ARGS;

void setBuildOption(char *opt) {
    if (PRO_ARGS->noBuild) {
        strcpy(opt, "test-without-building");
    }
    else {
        strcpy(opt, "build-for-testing");
    }
}

void setDevice() { /* no op */ }

int buildCommand(char* commandBuffer) {
    if (validateCommand() == 0) {
        ulog(warning, "Command failed to validate");
        return 0;
    }
    
    char build[128];
    setBuildOption(build);
    
    sprintf(
        commandBuffer,
        "xcodebuild test -scheme %s -sdk iphonesimulator -destination 'platform=iOS Simulator,name=%s,OS=%s' -only-testing %s",
        PRO_ARGS->scheme, PRO_ARGS->device, PRO_ARGS->os, PRO_ARGS->testTargetFile
    );
    
    return 1;
}

int validateCommand() {
    return 1;
}
