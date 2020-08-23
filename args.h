#ifndef ARGS_DOT_H
#define ARGS_DOT_H

void init_args();
int parseArgs(int argc, char** argv);
void args_setMatchingType(char * matchingString, char type);
void args_describe();

#endif /* ARGS_DOT_H */
