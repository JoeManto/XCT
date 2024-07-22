#include "xct.h"
#include "TestSuiteParsing/fparse_testsuite.h"
#include "TestCaseMatching/matcher.h"
#include "Exec/exec_runner.h"
#include "Environment/env_context.h"
#include "Environment/env_content_writer.h"
#include "Arguments/arg_set.h"

extern Arguments* PRO_ARGS;

void showProgramArgs(uint argc, char** argv) {
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

    // Merge stored args with cli args. Preferring cli.
    args_merge(storedArgs, PRO_ARGS);
    args_describe();

    if (!util_containsProjectOrWorkspace(".")) {
        ulog(warning, "Missing project or workspace at current directory");
    }

    env_save_short_term_context();
    //exec_run();

    fparse_dealloc();
    dealloc_args();
    
    return 0;
}

