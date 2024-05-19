//
//  env_content_writer.c
//  XCT-Product
//
//  Created by Joe Manto on 5/18/24.
//

#include "env_content_writer.h"
#include "env_context.h"
#include "../Arguments/args.h"

extern Arguments* PRO_ARGS;

void env_context_writer(char* path, Arguments* args);
int env_write_argument(ContextArgumentType argument_type, char* value, char* buffer);

void env_save_short_term_context(void) {
    env_context_writer("/tmp/.xctrc-test-short", PRO_ARGS);
}

void env_context_writer(char* path, Arguments* args) {
    FILE* file;
    if ((file = fopen(path, "w+")) == NULL) {
        exitOnError("Failed to open argument context file for writing", 0);
    }

    size_t total_bytes_written = 0;
    char* buffer = malloc(sizeof(char) * 250);
    char* buffer_ptr = buffer;

    ContextArgumentType arg_types[ENV_CONTEXT_ARG_COUNT] = { projectTarget, fileTarget, scheme, os, device };

    for(int i = 0; i < ENV_CONTEXT_ARG_COUNT; i++) {
        ContextArgumentType arg_type = arg_types[i];
        if (args->savedArguments[i] == unknown) {
            continue;
        }

        int bytes_written = 0;
        switch (arg_type) {
            case projectTarget:
                bytes_written = env_write_argument(arg_type, args->projectTarget, buffer_ptr);
                break;
            case fileTarget:
                bytes_written = env_write_argument(arg_type, args->testTargetFile, buffer_ptr);
                break;
            case scheme:
                bytes_written = env_write_argument(arg_type, args->scheme, buffer_ptr);
                break;
            case os:
                bytes_written = env_write_argument(arg_type, args->os, buffer_ptr);
                break;
            case device:
                bytes_written = env_write_argument(arg_type, args->device, buffer_ptr);
                break;
            case unknown:
                break;
        }

        if (bytes_written > 0 && i != ENV_CONTEXT_ARG_COUNT - 1) {
            buffer_ptr += bytes_written;
            total_bytes_written += bytes_written;
        }
    }

    if (fwrite(buffer, sizeof(char), total_bytes_written, file) == 0) {
        ulog(warning, "Nothing written to enviorment context file");
    }

    fflush(file);
    fclose(file);
    free(buffer);
}

int env_write_argument(ContextArgumentType argument_type, char* value, char* buffer) {
    if (value == NULL) {
        return 0;
    }

    size_t value_length = strlen(value);
    strcpy(buffer, value);
    strcat(buffer, "=");

    char* argument_key = args_getContextArgumentTypeKey(argument_type);
    size_t key_length = strlen(argument_key);
    strcat(buffer, argument_key);
    strcat(buffer, "\n");

    return (int)(value_length + key_length) + 2;
}
