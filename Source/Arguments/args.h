#ifndef ARGS_DOT_H
#define ARGS_DOT_H

#define ENV_CONTEXT_ARG_COUNT 6

enum Matcher {
  exact,
  substring,
  regex,
};

enum Project {
  workspace,
  project,
};

enum ContextArgumentType {
    projectTarget = 1,
    fileTarget = 2,
    scheme = 3,
    os = 4,
    device = 5,
    unknown = 6
} typedef ContextArgumentType;

typedef struct Arguments {
    enum Matcher matchingType;
    enum Project projectType;
    char* testTargetFile;
    char* projectTarget;
    char* matchingString;
    char* scheme;
    char* os;
    char* device;
    ContextArgumentType savedArguments[ENV_CONTEXT_ARG_COUNT];
    int dry;
    int ugly;
    int clear;
    int noBuild;
} Arguments;

void init_args(void);
void dealloc_args(void);
Arguments* newArgs(void);

int parseArgs(int argc, char** argv);
void args_describe(void);

void args_setArgumentsForString(Arguments* args, char* argumentString);
char* args_getContextArgumentTypeKey(ContextArgumentType type);
ContextArgumentType args_getContextArgumentForKey(char* key);
ContextArgumentType args_getContentArgumentForLabel(char label);
char* args_argumentComponent(ContextArgumentType argument, char* value);
Arguments* args_merge_new(Arguments* long_term, Arguments* short_term);
void args_merge(Arguments* long_term, Arguments* short_term);

#endif /* ARGS_DOT_H */
