#include "args.h"
#include "xct.h"

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
    char* testTargetFile;
    char* projectTarget;
    char* scheme;
    char* os;
    char* device;
    int dry
    int ugly
    int clear
    enum Matcher testMatcherType;
    enum Project projectType;
} args



int parseArgs(int* argc, char** argv){
    int opt;
    while((opt = getopt(*argc, argv, ":if:lrx")) != -1) {
        switch(opt) {
            case 'i':
            case 'l':
            case 'r':
                printf("option: %c\n", opt);
                break;
            case 'f':
                printf("filename: %s\n", optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    return 0;
}
