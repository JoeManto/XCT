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

    env_save_short_term_context();
   
    if (args_needsFileParsing(PRO_ARGS)) {
        ulogFormat(info, FILENAME_MAX, "Starting test file processing %s", PRO_ARGS->testTargetFile);

        if (!util_containsProjectOrWorkspace(".")) {
            exitOnError("Utilizing non-exact matching types require XCT to be executed from the root of the project or workspace.", errno);
        }

        fparse_init();
        fparse_start();
    }

    //exec_run();

    fparse_dealloc();
    dealloc_args();
    return 0;
}




