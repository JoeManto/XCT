#ifndef ARGS_DOT_H
#define ARGS_DOT_H

#include <inttypes.h>

#define ENV_CONTEXT_ARG_COUNT 7
#define SAVE_TOKEN '@'

enum Matcher {
  exact = 0,
  substring,
  regex,
};

enum Project {
  workspace = 0,
  project,
};

enum ContextArgumentType {
    projectTarget = 0,
    fileTarget,
    target,
    scheme,
    os,
    device,
    unknown
} typedef ContextArgumentType;

typedef struct Arguments {
    enum Matcher matchingType;
    enum Project projectType;
    char* testTargetFile;
    char* projectTarget;
    char* matchingString;
    char* target;
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

uint parseArgs(uint argc, char** argv);
void args_describe(void);

void args_setArgumentsForString(Arguments* args, char* argumentString);
char* args_getContextArgumentTypeKey(ContextArgumentType type);
char* args_getContextArgumentValueForKey(ContextArgumentType type, Arguments* args);
ContextArgumentType args_getContextArgumentTypeForKey(char* key);
ContextArgumentType args_getContentArgumentForLabel(char label);
char* args_argumentComponent(ContextArgumentType argument, char* value);
Arguments* args_merge_new(Arguments* long_term, Arguments* short_term);
void args_merge(Arguments* long_term, Arguments* short_term);
uint8_t args_needsFileParsing(Arguments* args);

#endif /* ARGS_DOT_H */
