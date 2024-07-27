//
//  file+util.h
//  XCT-Product
//
//  Created by Joe Manto on 7/20/24.
//

#ifndef file_util_h
#define file_util_h

#include <stdio.h>
#include <dirent.h>
#include "../xct.h"

uint8_t util_findFile(char* basePath, char* pathBuffer, char* fileName);
uint8_t util_containsProjectOrWorkspace(char* path);
uint8_t util_isProjectFile(char* filePath);
uint8_t util_isWorkspaceFile(char* filePath);
uint8_t util_copyFile(char* destinationPath, char* sourcePath);
uint8_t util_removingFileExtension(char* filePath);

#endif /* file_util_h */
