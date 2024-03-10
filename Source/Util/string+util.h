//
//  string+util.h
//  XCT-Product
//
//  Created by Joe Manto on 3/9/24.
//

#ifndef string_util_h
#define string_util_h

#include <stdio.h>

void freeStr(char* ptr);
void strcpy_ignore(char* dst, char* src, char* ignoreSet);
void strlower(char* str);

#endif /* string_util_h */
