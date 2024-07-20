//
//  env_context_parser.h
//  XCT-Product
//
//  Created by Joe Manto on 3/9/24.
//

#ifndef env_context_parser_h
#define env_context_parser_h

#include "../xct.h"

uint env_parseContextFile(char* path, Arguments* args);
char* buildArgumentString(char** components, uint numComponents);

#endif /* env_context_parser_h */
