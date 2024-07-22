//
//  file+util.c
//  XCT-Product
//
//  Created by Joe Manto on 7/20/24.
//

#include "file+util.h"
#include <dirent.h>

/// Assigns the provided buffer with the path of the file if a recursive match is found starting from the provided base path.
/// Returns a boolean indicating if the buffer was written to.
/// - Note: This implementation expects an already zero'd buffer.
uint8_t util_findFile(char* basePath, char* pathBuffer, char* fileName) {
    DIR *handle;
    struct dirent *directory = NULL;

    if ((handle = opendir(basePath))) {
        while ((directory = readdir(handle)) != NULL) {
            if (directory->d_type == DT_DIR
                && strcmp(directory->d_name,".") != 0
                && strcmp(directory->d_name,"..") != 0) {
                // Search nested directory.
                char sub_directory_path[FILENAME_MAX];
                sprintf(sub_directory_path, "%s/%s", basePath, directory->d_name);
                if (util_findFile(sub_directory_path, pathBuffer, fileName)) {
                    // Exit early if we found the file in nested directory.
                    break;
                }
            }

            if (strcmp(directory->d_name, fileName) == 0) {
                sprintf(pathBuffer, "%s/%s", basePath, directory->d_name);
                closedir(handle);
                return 1;
            }
        }
    }

    closedir(handle);
    return 0;
}

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
