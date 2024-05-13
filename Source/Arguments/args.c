#include "args.h"
#include "arg_set.h"
#include "../xct.h"
#include <getopt.h>

extern int opterr;

Arguments* PRO_ARGS;
ContextArgumentType contextArgTypes[ENV_CONTEXT_ARG_COUNT] = { projectTarget, fileTarget, scheme, os, device };

Arguments* newArgs(void) {
    Arguments* arguments = malloc(sizeof(Arguments));
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
    optind = 0;

    struct option long_options[] = {
        {"no-build", no_argument, &PRO_ARGS->noBuild, 1}
    };

    int option_index = 0;
    char option;
    
    while ((option = getopt_long(argc, argv, "t:e:s:d:P:S:O:D:", long_options, &option_index)) != EOF) {
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

void args_setArgumentsForString(Arguments* args, char* argumentString) {
    Arguments* originalArgs = PRO_ARGS;

    // arg1, arg2, arg3 + dummypath
    uint64_t numArgs = (strcount(',', argumentString) + 1) + 1;
    char** argv = malloc(sizeof(char *) * numArgs);

    char* path = "dummy_path";
    unsigned long pathSize = strlen(path);
    argv[0] = malloc(((sizeof(char) * pathSize)) + 1);
    strcpy(argv[0], path);

    char* token = strtok(argumentString, ",");
    int index = 1;
    while (token) {
        argv[index] = malloc((sizeof(char) * strlen(token)) + 1);
        strcpy(argv[index], token);

        index++;
        token = strtok(NULL, ",");
    }

    PRO_ARGS = args;
    parseArgs(index, argv);
    
    for (; index >= 0; --index) {
        free(argv[index]);
    }
    free(argv);

    PRO_ARGS = originalArgs;
}

char* args_argumentComponent(ContextArgumentType argument, char* value) {
    unsigned long valueSize;
    if ((valueSize = strlen(value)) <= 0) {
        return NULL;
    }

    unsigned long prefixSize = 3; // "-p "
    char* component = malloc((sizeof(char) * (valueSize + prefixSize)) + 1);

    switch (argument) {
        case projectTarget:
            strcpy(component, "-P,");
            break;
        case fileTarget:
            strcpy(component, "-t,");
            break;
        case scheme:
            strcpy(component, "-S,");
            break;
        case os:
            strcpy(component, "-O,");
            break;
        case device:
            strcpy(component, "-D,");
            break;
        case unknown:
            return NULL;
    }

    strcat(component, value);
    return component;
}

ContextArgumentType args_getContextArgumentForKey(char* key) {
    for (int i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        if (strcmp(key, args_getContextArgumentTypeKey(contextArgTypes[i])) == 0) {
            return contextArgTypes[i];
        }
    }

    return unknown;
}

Arguments* args_merge_new(Arguments* long_term, Arguments* short_term) {
    Arguments* originalArgs = PRO_ARGS;
    Arguments* merged = newArgs();
    PRO_ARGS = merged;

    args_merge(long_term, short_term);

    PRO_ARGS = originalArgs;
    return merged;
}

void args_merge(Arguments* long_term, Arguments* short_term) {
    for (int i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        switch (contextArgTypes[i]) {
            case projectTarget:
                if (short_term->projectTarget) {
                    args_setProjectProps(short_term->projectTarget);
                    break;
                }

                if (long_term->projectTarget) {
                    args_setProjectProps(long_term->projectTarget);
                    break;
                }

                break;
            case fileTarget:
                if (short_term->testTargetFile) {
                    args_setTestTargetFileProp(short_term->testTargetFile);
                    break;
                }

                if (long_term->testTargetFile) {
                    args_setTestTargetFileProp(long_term->testTargetFile);
                    break;
                }

                break;
            case scheme:
                if (short_term->scheme) {
                    args_setSchemeProp(short_term->scheme);
                    break;
                }

                if (long_term->scheme) {
                    args_setSchemeProp(long_term->scheme);
                    break;
                }

                break;
            case os:
                if (short_term->os) {
                    args_setOSProp(short_term->os);
                    break;
                }

                if (long_term->os) {
                    args_setOSProp(long_term->os);
                    break;
                }

                break;
            case device:
                if (short_term->device) {
                    args_setDeviceProp(short_term->device);
                    break;
                }

                if (long_term->device) {
                    args_setDeviceProp(long_term->device);
                    break;
                }

                break;
            case unknown:
                break;
        }
    }
}

char* args_getContextArgumentTypeKey(ContextArgumentType type) {
    switch (type) {
        case projectTarget:
            return "project";
        case fileTarget:
            return "file";
        case scheme:
            return "scheme";
        case os:
            return "os";
        case device:
            return "device";
        case unknown:
            return "unknown";
    }
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
