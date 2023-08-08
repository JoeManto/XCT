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
    int noBuild;
} args;

void init_args(void);
void dealloc_args(void);
void args_assignStringProp(char** dst, char* src);
void args_describe(void);
args* newArgs(void);
int parseArgs(int argc, char** argv);


#endif /* ARGS_DOT_H */
