#include "args.h"
#include "xct.h"

extern int opterr;
args * PRO_ARGS;

args* newArgs() {
  args* arguments = malloc(sizeof(struct Arguments));
  if(arguments == NULL){
    exitOnError("Memory Error");
  }

  // Alloc defaults
  arguments->dry = 0;
  arguments->ugly = 0;
  arguments->clear = 0;

  return arguments;
}

void init_args() {
  PRO_ARGS = newArgs();
}

void deinit_args() {
  free(PRO_ARGS);
  PRO_ARGS = NULL;
}

int parseArgs(int argc, char** argv) {
    init_args();

    int opt;
    // Set opterr to avoid external errors
    opterr = 0;

    while((opt = getopt(argc, argv, ":t:e:s:d:P:S:O:D:")) != EOF) {
        switch(opt) {
            case 'e':
            case 's':
            case 'd':
                printf("match option: %c\n", opt);
                opterr = args_setMatchingProps(optarg, opt);
                break;
            case 't':
                printf("testTargetFile: %s\n", optarg);
                break;
            case 'P':
                printf("Project: %s\n", optarg);
                break;
            case 'S':
                printf("scheme: %s\n", optarg);
                break;
            case 'O':
                printf("os: %s\n", optarg);
                break;
            case 'D':
                printf("device: %s\n", optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
            default:
              abort();
        }

        if(!opterr){
          return opterr;
        }
    }

    args_describe();
    return 0;
}

int args_setMatchingProps(char * matchingString, char type) {
  if(matchingString == NULL) {
      return 1;
  }

  // Set matchingType enum in args
  enum Matcher matcher = regex;
  switch(type){
    case 'e':
      matcher = exact;
    break;
    case 's':
      matcher = substring;
    break;
  };

  PRO_ARGS->matchingType = matcher;

  clearPropIfNeeded(PRO_ARGS->matchingString);
  args_assignStringProp(PRO_ARGS->matchingString, matchingString);

  return 0;
}

int args_setTestTargetFileProp(char * target) {
    if(target == NULL){
      return 1;
    }
    clearPropIfNeeded(PRO_ARGS->testTargetFile);
    args_assignStringProp(PRO_ARGS->testTargetFile, target);
}

int args_setProjectProps(char * projectTarget) {
    if(target == NULL){
      return 1;
    }

    char * tok = strtok(string, ".");
    tok = strtok(NULL, ".");

    // ProjectTarget doesnt have required delim
    if(tok == NULL){
      return 1;
    }

    // Set args projectType enum to the correct value
    char * projectType = tok;
    if(strcmp(projectType, "xcodeproj")){
        PRO_ARGS->projectType = project;
    }else if (strcmp(projectType, "xcworkspace")){
        PRO_ARGS->projectType = workspace;
    }else{
      return 1;
    }

    clearPropIfNeeded(PRO_ARGS->projectTarget);
    args_assignStringProp(PRO_ARGS->projectTarget, projectTarget);
}

int args_assignStringProp(char* dst, char* src){
  dst = malloc(strlen(src) * sizeof(char));
  strcpy(dst, src);
}

// Expects value to be not be NULL
// free and nullifies the given ptr
void args_clearPropIfNeeded(char * ptr) {
  if(ptr != NULL){
    free(ptr);
    ptr = NULL;
  }
}

void args_describe() {
  if(PRO_ARGS == NULL){
     printf("%s\n","Can't print args: PRO_ARGS is NULL");
     return;
  }
  printf("%s\n","-----------ARGS------------");
  printf("matchingType: %d\n",PRO_ARGS->matchingType);
  printf("matchingString: %s\n",PRO_ARGS->matchingString != NULL ? PRO_ARGS->matchingString : "(null)");
}
