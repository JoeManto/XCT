//
//  file+util.c
//  XCT-Product
//
//  Created by Joe Manto on 7/20/24.
//

#include "file+util.h"

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
                memset(sub_directory_path, 0, FILENAME_MAX);
                sprintf(sub_directory_path, "%s/%s", basePath, directory->d_name);
                if (util_findFile(sub_directory_path, pathBuffer, fileName)) {
                    // Exit early if we found the file in nested directory.
                    closedir(handle);
                    return 1;
                }
            }

            char* name = malloc(sizeof(char) * strlen(directory->d_name));
            strcpy(name, directory->d_name);
            util_removingFileExtension(name);

            if (directory->d_type != DT_DIR && strcmp(name, fileName) == 0) {
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

uint8_t util_copyFile(char* destinationPath, char* sourcePath) {
    FILE* sourceHandle = fopen(sourcePath, "r");
    if (!sourceHandle) {
        ulogFormat(error, FILENAME_MAX, "Failed to open source path %s", sourcePath);
        return 0;
    }

    FILE* destinationHandle = fopen(destinationPath, "w+");
    if (!destinationHandle) {
        ulogFormat(error, FILENAME_MAX, "Failed to open destination path %s", destinationPath);
        fclose(sourceHandle);
        return 0;
    }

    uint8_t success = 1;

    char character;
    while ((character = fgetc(sourceHandle)) != EOF) {
        if (ferror(sourceHandle)) {
            ulog(error, "Error reading source file");
            success = 0;
            break;
        }

        if (feof(sourceHandle)) {
            ulog(info, "Reached end of source file");
            break;
        }

        if (fputc(character, destinationHandle) == EOF) {
            ulog(error, "Error while occurred while copying source file");
            success = 0;
            break;
        }
    }

    fclose(sourceHandle);
    fflush(destinationHandle);
    fclose(destinationHandle);

    return success == 1;
}

uint8_t util_removingFileExtension(char* filePath) {
    char* extensionIndicator = strrchr(filePath, '.');
    if (extensionIndicator) {
        *extensionIndicator = '\0';
        return 1;
    }
    
    return 0;
}
