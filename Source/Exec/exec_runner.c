#include "exec_runner.h"
#include "exec_command.h"
#include "../xct.h"
#include <dirent.h>

extern Arguments* PRO_ARGS;

int runCommandWithOutput(char* command) {
    FILE *fp;
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return 0;
    }
    
    char* outputBuffer = malloc(sizeof(char) * 256 + 1);
    int i = 0;
    while (fgets(outputBuffer, sizeof(outputBuffer), fp) != NULL) {
        if (i > 1) {
            outputBuffer = realloc(outputBuffer, sizeof(char) * (256 * i));
            printf("%s", outputBuffer);
        }
        i++;
    }
    return 1;
}

int exec_run(void) {
    char command[2028];
    int result;
    if ((result = buildCommand(command)) != 0) {
        ulog(error, "Failed to build command");
        return result;
    }

    printf("Running Command:\n%s\n\n", command);
    runCommandWithOutput(command);
    return 1;
}
