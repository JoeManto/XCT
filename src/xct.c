#include "xct.h"
#include "args/args.h"
#include "testSuiteParsing/fparse_testsuite.h"
#include "testCaseMatching/matcher.h"
#include "exec/exec_runner.h"

extern args* PRO_ARGS;

void removeXcodeArgs(int* argc, char** argv) {
    int lastIndex = *argc - 1;
    
    argv[lastIndex] = NULL;
    argv[lastIndex - 1] = NULL;
    *argc -= 2;
}

void showProgramArgs(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        if (argv[i]) {
            printf("arg[%d]:'%s'\n", i, argv[i]);
        }
    }
}

int main(int argc, char** argv) {
    removeXcodeArgs(&argc, argv);
    showProgramArgs(argc, argv);
    
    init_args();
    parseArgs(argc, argv);
    
    //exec_run();
    
    fparse_dealloc();
    dealloc_args();
    
    return 0;
}

