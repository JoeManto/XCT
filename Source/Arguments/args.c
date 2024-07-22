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

    for (uint i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        arguments->savedArguments[i] = unknown;
    }

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

uint parseArgs(uint argc, char** argv) {
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

        bool hasSavePrefix = false;
        if (optarg != NULL && optarg[0] == SAVE_TOKEN) {
            hasSavePrefix = true;
            optarg += 1; // Strip the '@'.
        }

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
                continue;
            case '?':
                printf("unknown option: %c\n", optopt);
                continue;
            default:
                continue;
        }

        if (hasSavePrefix) {
            ContextArgumentType type = args_getContentArgumentForLabel(option);
            args_setSavedArgument(type);
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

/// Creates new arguments by merging `short_term` arguments into `long_term`.
/// Where the existing values are replaced.
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

/// Returns the key string for the provided context argument type.
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

/// Returns the context argument type given the command line flag label.
ContextArgumentType args_getContentArgumentForLabel(char label) {
    switch(label) {
        case 'e':
        case 's':
        case 'd':
            return unknown;
        case 't':
            return fileTarget;
        case 'P':
            return projectTarget;
        case 'S':
            return scheme;
        case 'O':
            return os;
        case 'D':
            return device;
            break;
        default:
            return unknown;
    }
}


/// Returns the argument string mapping to the passed argument type.
char* args_getContextArgumentValueForKey(ContextArgumentType type, Arguments* args) {
    switch (type) {
        case projectTarget:
            return args->projectTarget;
        case fileTarget:
            return args->testTargetFile;
        case scheme:
            return args->scheme;
        case os:
            return args->os;
        case device:
            return args->device;
        case unknown:
            return NULL;
    }
}

/// Returns the argument type if the provided key string exactly matches.
ContextArgumentType args_getContextArgumentTypeForKey(char* key) {
    for (uint i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        if (strcmp(key, args_getContextArgumentTypeKey(contextArgTypes[i])) == 0) {
            return contextArgTypes[i];
        }
    }

    return unknown;
}

uint8_t args_needsFileParsing(Arguments* args) {
    if (!args->testTargetFile) {
        return 0;
    }

    if (!args->matchingType) {
        return 0;
    }

    switch (args->matchingType) {
        case exact:
            return 0;
        case substring:
        case regex:
            return 1;
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
    printf("savedArgs:\n");

    for (int i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        printf("[%d]:%s\n", i, args_getContextArgumentTypeKey(PRO_ARGS->savedArguments[i]));
    }

    printf("%s\n", "---------------------------");
}
