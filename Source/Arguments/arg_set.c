#include "arg_set.h"
#include "args.h"
#include "../xct.h"

extern Arguments* PRO_ARGS;

void assignDefaultArgs(void) {
    args_setMatchingProps("none", 's');
    args_setTestTargetFileProp("none");
    args_setSchemeProp("none");
    args_setOSProp("none");
    args_setDeviceProp("none");
    args_setProjectProps("none");
    args_setNoBuild(0);
}

/// Sets the `matchingString` and `matchingType` arguments
/// Returns Bool indicating success
int args_setMatchingProps(char * matchingString, char type) {
    if (matchingString == NULL) {
        return 0;
    }
    
    enum Matcher matcher = regex;
    switch(type){
        case 'e':
            matcher = exact;
            break;
        case 's':
            matcher = substring;
            break;
    };
    
    args_assignStringProp(&PRO_ARGS->matchingString, matchingString);
    PRO_ARGS->matchingType = matcher;
    return 1;
}

/// Sets the `testTargetFile` argument
/// Returns Bool indicating success
int args_setTestTargetFileProp(char* target) {
    if (target == NULL) {
        return 0;
    }
    
    args_assignStringProp(&PRO_ARGS->testTargetFile, target);
    return 1;
}

/// Sets the `scheme` argument
/// Returns Bool indicating success
int args_setSchemeProp(char* scheme) {
    if (scheme == NULL) {
        return 0;
    }
    
    args_assignStringProp(&PRO_ARGS->scheme, scheme);
    return 1;
}

/// Sets the `os` argument
/// Returns Bool indicating success
int args_setOSProp(char* os) {
    if (os == NULL) {
        return 0;
    }
    
    args_assignStringProp(&PRO_ARGS->os, os);
    return 1;
}

/// Sets the `os` argument
/// Returns Bool indicating success
int args_setDeviceProp(char* device) {
    if (device == NULL) {
        return 0;
    }
    
    args_assignStringProp(&PRO_ARGS->device, device);
    return 1;
}

/// Sets the `no build` argument
void args_setNoBuild(int noBuild) {
    PRO_ARGS->noBuild = noBuild;
}

/// Sets the `project` argument
/// Returns Bool indicating success
int args_setProjectProps(char* projectTarget) {
    if (projectTarget == NULL) {
        return 0;
    }
    
    args_assignStringProp(&PRO_ARGS->projectTarget, projectTarget);
    
    /*
    if (projectTarget == NULL) {
        return 1;
    }
    
    char* extension;
    if ((extension = strchr(projectTarget, '.')) == NULL) {
        // ProjectTarget doesnt have required delim
        return 1;
    }
    
    // Set args projectType enum to the correct value
    if (strcmp(extension, ".xcodeproj") == 0) {
        PRO_ARGS->projectType = project;
    }
    else if (strcmp(extension, ".xcworkspace") == 0) {
        PRO_ARGS->projectType = workspace;
    }
    else{
        // Invalid ProjectTarget
        return 1;
    }*/
    
    //args_assignStringProp(&PRO_ARGS->projectTarget, projectTarget);
    return 1;
}

/// Assigns any string arg
/// Note: Passed value no longer needs to be maintained by caller and should be freed if dynamically allocated
void args_assignStringProp(char** dst, char* src) {
    char* srccpy = malloc(sizeof(char) * strlen(src) + 1);
    strcpy(srccpy, src);
    
    *dst = malloc(sizeof(char) * strlen(srccpy) + 1);
    strcpy(*dst, srccpy);
    printf("dst: %s\n", *dst);
    //if (src[0] == ' ') {
    //    strcpy(dst, src+1);
   // }
   // else{
        
   // }

    free(srccpy);
}
