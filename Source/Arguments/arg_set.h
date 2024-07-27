#ifndef ARG_SET_DOT_H
#define ARG_SET_DOT_H

#include "../Arguments/args.h"
#include <inttypes.h>

uint8_t args_setMatchingProps(char* matchingString, char type);
uint8_t args_setProjectProps(char* projectTarget);
uint8_t args_setTestTargetFileProp(char* target);
uint8_t args_setTargetProp(char* target);
uint8_t args_setSchemeProp(char* scheme);
uint8_t args_setOSProp(char* os);
uint8_t args_setDeviceProp(char* device);
void args_setNoBuild(uint8_t noBuild);
void args_setSavedArgument(ContextArgumentType type);

#endif /* ARG_SET_DOT_H */
