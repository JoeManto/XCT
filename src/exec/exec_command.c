#include "exec_command.h"
#include "../xct.h"
#include "../args/args.h"

extern args* PRO_ARGS;

void setBuildOption(char *opt) {
    if (PRO_ARGS->noBuild) {
        strcpy(opt, "test-without-building");
    }
    else {
        strcpy(opt, "build-for-testing");
    }
}

void setDevice() {
    if (PRO_ARGS->device && strcmp(PRO_ARGS->device, "none")) {
        
    }
}

int buildCommand(char* commandBuffer) {
    if (validateCommand() == 0) {
        return 0;
    }
    
    char build[128];
    setBuildOption(build);
    
    sprintf(commandBuffer, "xcodebuild test -scheme %s -sdk iphonesimulator -destination 'platform=iOS Simulator,name=%s,OS=%s' -only-testing %s", PRO_ARGS->scheme, PRO_ARGS->device, PRO_ARGS->os, PRO_ARGS->testTargetFile
    );
    
    return 1;
}

int validateCommand() {
    return 1;
}
