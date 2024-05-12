#include "xct.h"
#include "TestSuiteParsing/fparse_testsuite.h"
#include "TestCaseMatching/matcher.h"
#include "Exec/exec_runner.h"
#include "Environment/env_context.h"
#include "Arguments/arg_set.h"

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

    Arguments* storedArgs = env_retrieveStoredArgs();
    PRO_ARGS = storedArgs;

    ulog(info, "Final args");
    args_describe();
    //exec_run();

    //ulog(info, "BEFORE");
    //args_describe();

    //args_assignStringProp(&PRO_ARGS->device, "My Device");

    //ulog(info, "AFTER");
    //args_describe();


    fparse_dealloc();
    dealloc_args();
    
    return 0;
}

