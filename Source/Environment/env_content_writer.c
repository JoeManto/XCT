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
extern Arguments* short_term_args;

void env_context_writer(char* path, Arguments* args);
int env_write_argument(ContextArgumentType argument_type, char* value, char* buffer);
char* env_argument_value(ContextArgumentType argument_type, Arguments* short_term_args, Arguments* main_args);

void env_save_short_term_context(void) {
    char* path = env_contextFilePath(".xctrc-short");
    env_context_writer(path, PRO_ARGS);

    free(path);
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

        char* value = env_argument_value(arg_type, short_term_args, args);

        if (value == NULL) {
            continue;
        }

        int bytes_written = env_write_argument(arg_type, value, buffer_ptr);

        if (bytes_written > 0 && i != ENV_CONTEXT_ARG_COUNT - 1) {
            buffer_ptr += bytes_written;
            total_bytes_written += bytes_written;
        }
    }

    if (fwrite(buffer, sizeof(char), total_bytes_written, file) == 0) {
        ulog(warning, "Nothing written to environment context file");
    }

    fflush(file);
    fclose(file);
    free(buffer);
}

char* env_argument_value(ContextArgumentType argument_type, Arguments* short_term_args, Arguments* main_args) {
    uint8_t saved_argument_index = (uint8_t) argument_type;
    char* saved_argument_value = args_getContextArgumentValueForKey(argument_type, main_args);
    char* short_term_argument_value = args_getContextArgumentValueForKey(argument_type, short_term_args);

    if (main_args->savedArguments[saved_argument_index] != unknown && saved_argument_value) {
        return saved_argument_value;
    } else if (short_term_argument_value) {
        return short_term_argument_value;
    }
    
    return NULL;
}

int env_write_argument(ContextArgumentType argument_type, char* value, char* buffer) {
    if (value == NULL) {
        return 0;
    }

    char* argument_key = args_getContextArgumentTypeKey(argument_type);
    size_t key_length = strlen(argument_key);
    strcpy(buffer, argument_key);
    strcat(buffer, "=");

    size_t value_length = strlen(value);
    strcat(buffer, value);
    strcat(buffer, "\n");
    
    ulogFormat(info, 25, "Saved arg %s", argument_key);
    return (int)(value_length + key_length) + 2;
}
