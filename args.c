#include "args.h"
#include "xct.h"

extern int opterr;

enum Matcher {
  exact,
  substring,
  regex,
};

enum Project {
  workspace,
  project,
};

typedef struct Arguments {
    enum Matcher matchingType;
    enum Project projectType;
    char* testTargetFile;
    char* projectTarget;
    char* matchingString;
    char* scheme;
    char* os;
    char* device;
    int dry;
    int ugly;
    int clear;
} args;

args * PRO_ARGS;

args* newArgs() {
  args* arguments = malloc(sizeof(struct Arguments));
  if(arguments == NULL){
    perror("Memory Error");
    exit(1);
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

int parseArgs(int argc, char** argv){

    init_args();

    int opt;
    opterr = 0;

    while((opt = getopt(argc, argv, "t:e:s:d:P:S:O:D:")) != EOF) {
        switch(opt) {
            case 'e':
            case 's':
            case 'd':
                printf("match option: %c\n", opt);
                args_setMatchingType(optarg, opt);
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
    }

    args_describe();
    return 0;
}

void args_setMatchingType(char * matchingString, char type){
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

  // If a matching string already exists free & nullify
  if(PRO_ARGS->matchingString != NULL){
    free(PRO_ARGS->matchingString);
    PRO_ARGS->matchingString = NULL;
  }

  PRO_ARGS->matchingString = malloc(strlen(matchingString) * sizeof(char));
  strcpy(PRO_ARGS->matchingString, matchingString);
}


void args_describe(){
  if(PRO_ARGS == NULL){
     printf("%s\n","Can't print args: PRO_ARGS is NULL");
     return;
  }
  printf("%s\n","-----------ARGS------------");

  printf("matchingType: %d\n",PRO_ARGS->matchingType);
  printf("matchingString: %s\n",PRO_ARGS->matchingString != NULL ? PRO_ARGS->matchingString : "(null)");
}
