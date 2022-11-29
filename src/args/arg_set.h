#ifndef ARG_SET_DOT_H
#define ARG_SET_DOT_H


int args_setMatchingProps(char* matchingString, char type);
int args_setProjectProps(char * projectTarget);
int args_setTestTargetFileProp(char * target);
int args_setSchemeProp(char* scheme);
int args_setOSProp(char* os);
int args_setDeviceProp(char* device);
void args_setNoBuild(int noBuild);
void assignDefaultArgs();


#endif /* ARG_SET_DOT_H */
