//
//  file+util.c
//  XCT-Product
//
//  Created by Joe Manto on 7/20/24.
//

#include "file+util.h"
#include <dirent.h>

uint8_t util_containsProjectOrWorkspace(char* path) {
    DIR *handle;
    struct dirent *directory;

    if ((handle = opendir(path))) {
        while ((directory = readdir(handle)) != NULL) {
            if (directory->d_type == DT_REG) {
                // Project and workspace files are not regular files, but directories.
                continue;
            }

            if (util_isProjectFile(directory->d_name)) {
                return 1;
            }

            if (util_isWorkspaceFile(directory->d_name)) {
                return 1;
            }
        }

        closedir(handle);
    }

    return 0;
}

uint8_t util_isProjectFile(char* filePath) {
    return str_hasExtension(filePath, "xcodeproj");
}

uint8_t util_isWorkspaceFile(char* filePath) {
    return str_hasExtension(filePath, "xcworkspace");
}
