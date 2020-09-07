#ifndef ARGS_DOT_H
#define ARGS_DOT_H

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

void init_args();
void dealloc_args();
void args_assignStringProp(char** dst, char* src);
void args_clearPropIfNeeded(char** ptr);
void args_describe();
args* newArgs();
int parseArgs(int argc, char** argv);
int args_setMatchingProps(char* matchingString, char type);
int args_setProjectProps(char * projectTarget);
int args_setTestTargetFileProp(char * target);
int args_setSchemeProp(char* scheme);
int args_setOSProp(char* os);
int args_setDeviceProp(char* device);


#endif /* ARGS_DOT_H */
