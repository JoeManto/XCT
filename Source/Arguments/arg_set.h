#ifndef ARG_SET_DOT_H
#define ARG_SET_DOT_H

#include "../Arguments/args.h"

int args_setMatchingProps(char* matchingString, char type);
int args_setProjectProps(char * projectTarget);
int args_setTestTargetFileProp(char * target);
int args_setSchemeProp(char* scheme);
int args_setOSProp(char* os);
int args_setDeviceProp(char* device);
void args_setNoBuild(int noBuild);
void args_setSavedArgument(ContextArgumentType type);

#endif /* ARG_SET_DOT_H */
