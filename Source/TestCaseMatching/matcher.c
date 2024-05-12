#include "matcher.h"
#include "../xct.h"
#include <regex.h>

#define DEFAULT_MATCH_SIZE 10
#define TESTCASE_PATTERN "\w*func test"

extern Arguments* PRO_ARGS;

int num_matches;
match* MATCHES;

void matcher_init(void) {
    num_matches = 0;
    MATCHES = malloc(sizeof(struct Match) * DEFAULT_MATCH_SIZE);
}

void matcher_dealloc(void) {
    if (MATCHES) {
        free(MATCHES);
    }
    MATCHES = NULL;
}

/// Attempts to find pattern in given string.
/// Provide integer ptr to capture index in given string which the pattern starts
/// @Returns 1 on match success otherwise 0
int matcher_match(char * string, char * pattern, int * foundAtIndex) {
    regex_t    preg;
    int        rc;
    size_t     nmatch = 1;
    regmatch_t pmatch[2];
    
    if (0 != (rc = regcomp(&preg, pattern, REG_EXTENDED))) {
        ulog(warning, "Failed to compile regex expression. Parsing could be effected");
        return 1;
    }
    
    if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
        printf("Failed to match line:'%s' with pattern:'%s' error:%d\n", string, pattern, rc);
        regfree(&preg);
        return 0;
    }
    else {
        printf("Matched line:'%s' with pattern:'%s'\n", string, pattern);
        if (foundAtIndex) {
            *foundAtIndex = pmatch[0].rm_eo - pmatch[0].rm_so;
        }

        regfree(&preg);
        return 1;
    }
    
    return rc;
}

/// Determines if a given line contains a XCTestCase
/// @returns The starting index in the curline in which the header was found, otherwise < 0
int matcher_containsTestHeader(char* line) {
    char* pattern = TESTCASE_PATTERN;
    int testCasePadding = -1;
    int isMatch = matcher_match(line, pattern, &testCasePadding);
    
    return testCasePadding;
}

char* trimTestCase(char * testCase, int testCasePadding) {
    int offset = testCasePadding + strlen("test");
    testCase += offset;
    
    offset = 0;
    for (;testCase[offset] != '('; offset++) {
        if (testCase[offset] == EOF) {
            break;
        }
    }
    testCase[offset] = '\0';
    return testCase;
}

int matcher_regex(char* testCase) {
    printf("SDFSFDSDF");
    return 1;
}

// Checks if matchingString is contained in the testCase
int matcher_substring(char* testCase) {
    if (strstr(testCase, PRO_ARGS->matchingString)) {
        return 1;
    }
    return 0;
}

int matcher_exact(char* testCase) {
    if (!PRO_ARGS->matchingString) {
        return 0;
    }
    return strcmp(testCase, PRO_ARGS->matchingString) == 0;
}

int matcher_doesTestCaseMatch(char* testCase, int testCasePadding) {
    char* testCaseCopy = malloc(sizeof(char) * strlen(testCase));
    strcpy(testCaseCopy, testCase);
    
    int isMatch = 0;
    testCase = trimTestCase(testCaseCopy, testCasePadding);
    
    switch (PRO_ARGS->matchingType) {
        case regex:
            isMatch = matcher_regex(testCaseCopy);
            break;
        case substring:
            isMatch = matcher_substring(testCaseCopy);
            break;
        case exact:
            isMatch = matcher_exact(testCaseCopy);
            break;
    }
    printf("testcase: '%s'\n", testCaseCopy);
    printf("matching type:'%i'\n", PRO_ARGS->matchingType);
    printf("matching: '%s'\n", PRO_ARGS->matchingString);
    printf("isMatch: '%s'\n", isMatch ? "Yes" : "No");
    
    free(testCaseCopy);
    return isMatch;
}

void matcher_appendMatch(char* line, int line_start, int line_end) {
    // check if more space is needed
    // If so extend by a 10 matches
    if (num_matches % 10 == 0) {
        match* temp = realloc(MATCHES, sizeof(struct Match) * (num_matches + DEFAULT_MATCH_SIZE));
        if (temp == NULL) {
            exitOnError("Memory Error", ENOMEM);
        }
        else {
            MATCHES = temp;
        }
    }
    num_matches++;
}
