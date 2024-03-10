#include "xct.h"
#include "TestSuiteParsing/fparse_testsuite.h"
#include "TestCaseMatching/matcher.h"
#include "Exec/exec_runner.h"
#include "Environment/env_context.h"

extern Arguments* PRO_ARGS;

void showProgramArgs(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        if (argv[i]) {
            printf("arg[%d]:'%s'\n", i, argv[i]);
        }
    }
}

int main(int argc, char** argv) {
    showProgramArgs(argc, argv);
    init_args();
    parseArgs(argc, argv);

    env_retrieveStoredArgs();
    //exec_run();
    
    fparse_dealloc();
    dealloc_args();
    
    return 0;
}

