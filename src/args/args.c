#include "args.h"
#include "arg_set.h"
#include "../xct.h"
#include <getopt.h>

extern int opterr;
args * PRO_ARGS;

args* newArgs(void) {
    args* arguments = malloc(sizeof(args));
    if(arguments == NULL){
        exitOnError("Memory Error", ENOMEM);
    }
    
    arguments->testTargetFile = NULL;
    arguments->projectTarget = NULL;
    arguments->matchingString = NULL;
    arguments->scheme = NULL;
    arguments->os = NULL;
    arguments->device = NULL;
    
    return arguments;
}

void init_args(void) {
    PRO_ARGS = newArgs();
}

void dealloc_args(void) {
    if (PRO_ARGS) {
        free(PRO_ARGS);
    }
    PRO_ARGS = NULL;
}

int parseArgs(int argc, char** argv) {
    if (PRO_ARGS == NULL) {
        init_args();
    }

    // Set opterr to avoid external errors
    opterr = 0;
        
    struct option long_options[] = {
        {"no-build", no_argument, &PRO_ARGS->noBuild, 1}
    };
    
    int option_index = 0;
    char option;
    
    while ((option = getopt_long(argc, argv, ":t:e:s:d:P:S:O:D:", long_options, &option_index)) != EOF) {
        switch(option) {
            case 'e':
            case 's':
            case 'd':
                printf("set match option: %c\n", option);
                opterr = args_setMatchingProps(optarg, option);
                break;
            case 't':
                printf("set testTargetFile: %s\n", optarg);
                opterr = args_setTestTargetFileProp(optarg);
                break;
            case 'P':
                printf("set project: %s\n", optarg);
                opterr = args_setProjectProps(optarg);
                break;
            case 'S':
                printf("set scheme: %s\n", optarg);
                opterr = args_setSchemeProp(optarg);
                break;
            case 'O':
                printf("set os: %s\n", optarg);
                opterr = args_setOSProp(optarg);
                break;
            case 'D':
                printf("set device: %s\n", optarg);
                opterr = args_setDeviceProp(optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
            default:
                break;
        }
    }
    
    args_describe();
    
    if (opterr) {
        return opterr;
    }
    
    return 0;
}

void args_getDefaultDevice(void) {
    
}

void args_describe(void) {
    if (PRO_ARGS == NULL) {
        printf("%s\n","Can't print args: PRO_ARGS is NULL");
        return;
    }
        
    printf("%s\n","-----------ARGS------------");
    printf("matchingType: %d\n",PRO_ARGS->matchingType);
    printf("matchingString: %s\n",PRO_ARGS->matchingString);
    printf("testTargetFile: %s\n",PRO_ARGS->testTargetFile != NULL ? PRO_ARGS->testTargetFile : "(null)");
    printf("projectTarget: %s\n",PRO_ARGS->projectTarget != NULL ? PRO_ARGS->projectTarget : "(null)");
    printf("scheme: %s\n",PRO_ARGS->scheme != NULL ? PRO_ARGS->scheme : "(null)");
    printf("os: %s\n",PRO_ARGS->os != NULL ? PRO_ARGS->os : "(null)");
    printf("device: %s\n",PRO_ARGS->device != NULL ? PRO_ARGS->device : "(null)");
    printf("%s\n", "---------------------------");
}
