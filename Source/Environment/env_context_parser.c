//
//  env_context_parser.c
//  XCT-Product
//
//  Created by Joe Manto on 3/9/24.
//

#include "../xct.h"
#include "../Arguments/arg_set.h"
#include "env_context_parser.h"

char* env_parseContextLine(char* line, Arguments* args);
char* buildArgumentString(char** components, uint numComponents) ;
char* commandComponents[ENV_CONTEXT_ARG_COUNT];

ContextArgumentType* unusedComponents;

uint env_parseContextFile(char* path, Arguments* args) {
    FILE* file;
    if ((file = fopen(path, "r")) == NULL) {
        ulogFormat(error, 100, "Failed to open argument context file %s\n", path);
        exitOnError("Failed to open context file", 0);
    }

    ContextArgumentType components[ENV_CONTEXT_ARG_COUNT] = { projectTarget, fileTarget, scheme, os, device };
    unusedComponents = components;

    uint commandsIndex = 0;

    char* line_buffer = malloc(sizeof(char) * 100);
    while (line_buffer != NULL) {
        line_buffer = fgets(line_buffer, 100, file);

        if (ferror(file)) {
            ulog(error, "Error reading context file");
            break;
        }

        if (feof(file)) {
            ulog(info, "Reached end of context file");
            break;
        }

        if (line_buffer == NULL) {
            break;
        }
        
        char* command = env_parseContextLine(line_buffer, args);
        if (command) {
            commandComponents[commandsIndex++] = command;
        }

        printf("read line: %s\n", line_buffer);
    }

    if (commandsIndex > 0) {
        char* argumentString = buildArgumentString(commandComponents, commandsIndex);
        args_setArgumentsForString(args, argumentString);
    } else {
        ulog(info, "No settings found");
    }

    for (; commandsIndex > 0;) {
        if (commandComponents[--commandsIndex] != NULL) {
            free(commandComponents[commandsIndex]);
        }
    }

    free(line_buffer);
    fclose(file);
    return 0;
}

char* env_parseContextLine(char* line, Arguments* args) {
    char* token = strtok(line, "=");
    if (!token) {
        return NULL;
    }

    unsigned long key_size = strlen(token);
    char key[key_size];
    strcpy_ignore(key, token, " \"\n");
    strlower(key);

    token = strtok(NULL, "=");
    if (!token) {
        return NULL;
    }

    unsigned long value_size = strlen(token);
    char value[value_size];
    strcpy_ignore(value, token, "\"\n"); // Keep spaces

    printf("KEY: %s VALUE: %s\n", key, value);

    ContextArgumentType argument;
    if ((argument = args_getContextArgumentTypeForKey(key)) == unknown) {
        ulog(warning, "Found invalid key");
        return NULL;
    }

    bool alreadyInserted = true;
    for (int i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        if (unusedComponents[i] == argument) {
            alreadyInserted = false;
            break;
        }
    }

    if (alreadyInserted) {
        ulog(warning, "Setting already set");
        return NULL;
    }

    return args_argumentComponent(argument, value);
}

char* buildArgumentString(char** components, uint numComponents) {
    uint size = 0;
    for (uint i = 0; i < numComponents; i++) {
        size += strlen(components[i]);
    }

    char* argumentString = malloc((sizeof(char) * size) + 1);
    for (uint i = 0; i < numComponents; i++) {
        if (i > 0) {
            strcat(argumentString, ",");
        }
        strcat(argumentString, components[i]);
    }

    return argumentString;
}
