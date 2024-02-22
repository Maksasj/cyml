#include "cyml.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

CYMLTokenResult cyml_empty_token_result(CYMLResult code) {
    CYMLTokenResult result = {
        .code = code,
        .tokens = NULL,
        .tokenCount = 0
    };

    return result;
}

CYMLTokenResult cyml_error_token_result() {
    return cyml_empty_token_result(CYML_ERROR);
}

int cyml_utils_string_contains_character(const char* string, char character) {
    unsigned long length = strlen(string);

    for(unsigned long i = 0; i < length; ++i)
        if(string[i] == character)
            return 1;

    return 0;
}

char* cyml_utils_append_character(char* string, char character) {
    unsigned long length = strlen(string);
    string = realloc(string, length + 2);

    if(string == NULL)
        perror("Failed to reallocate string while appending character");

    string[length] = character;
    string[length + 1] = '\0';

    return string;
}

int cyml_utils_is_float(const char* str) {
    if(!cyml_utils_string_contains_character(str, '.'))
        return 0;

    char* endptr;
    errno = 0;
    float val = strtof(str, &endptr);

    if ((errno == ERANGE && (val == HUGE_VALF || val == HUGE_VALL)) || (errno != 0 && val == 0))
        return 0;

    if (endptr == str)
        return 0;

    return 1;
}

float cyml_utils_string_to_float(const char* str) {
    char* endptr;
    return strtof(str, &endptr);
}

int cyml_utils_is_u32(const char* str) {
    if (str == NULL || *str == '\0')
        return 0;

    while (*str != '\0') {
        if (!isdigit((unsigned char)*str))
            return 0;
        ++str;
    }

    char* endptr;
    errno = 0;
    unsigned long val = strtoul(str, &endptr, 10);

    if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0))
        return 0;

    return 1;
}

unsigned int cyml_utils_string_to_u32(const char* str) {
    char* endptr;
    return strtoul(str, &endptr, 10);
}

void cyml_free_token_result(CYMLTokenResult result) {
    for(unsigned int i = 0; i < result.tokenCount; ++i)
        if(result.tokens[i].value != NULL)
            free(result.tokens[i].value);

    free(result.tokens);
}

CYMLType cyml_parse_value_type_string(const char* string) {
    if(strcmp("TUPLE", string) == 0)
        return TYPE_TUPLE;
    else if(strcmp("STRING", string) == 0)
        return TYPE_STRING;
    else if(strcmp("FLOAT", string) == 0)
        return TYPE_FLOAT;
    else if(strcmp("U32", string) == 0)
        return TYPE_UNSIGNED_INT;

    return TYPE_UNDEFINED;
}

CYMLToken cyml_parse_token(const char* string) {
    unsigned long length = strlen(string);

    CYMLType type = cyml_parse_value_type_string(string);
    if(type != TYPE_UNDEFINED) {
        CYMLToken token = {
            .value = malloc(sizeof(CYMLType)),
            .type = TOKEN_TYPE
        };

        *((CYMLType *) token.value) = type;

        return token;
    } else if(strcmp("BEGIN", string) == 0) {
        CYMLToken token = {TOKEN_BEGIN, NULL};
        return token;
    } else if(strcmp("END", string) == 0) {
        CYMLToken token = {TOKEN_END, NULL};
        return token;
    } else if(string[0] == '"') {
        CYMLToken token = {
            .value = malloc(length - 2),
            .type = TOKEN_STRING
        };

        memcpy(token.value, string + 1, length - 2);

        return token;
    } else if(cyml_utils_is_float(string)) {
        CYMLToken token = {
            .value = malloc(sizeof(float)),
            .type = TOKEN_FLOAT
        };

        *((float *) token.value) = cyml_utils_string_to_float(string);

        return token;
    } else if(cyml_utils_is_u32(string)) {
        CYMLToken token = {
            .value = malloc(sizeof(unsigned int)),
            .type = TOKEN_UNSIGNED_INT
        };

        *((unsigned int *) token.value) = cyml_utils_string_to_u32(string);

        return token;
    } else {
        CYMLToken token = {
            .value = malloc(length + 1),
            .type = TOKEN_WORD
        };

        memcpy(token.value, string, length + 1);

        return token;
    }

    CYMLToken token = {TOKEN_UNDEFINED, NULL};
    return token;
}

CYMLTokenResult cyml_tokens_parse_string(const char* string) {
    const unsigned long length = strlen(string);

    CYMLTokenResult result = cyml_empty_token_result(CYML_OK);

    char* sub = NULL;

    int skip = 0;
    int scope = 0;

    for(unsigned long i = 0; i < length + 1; ++i) {
        char character = string[i];

        if(skip) {
            skip = 0;

            if(sub == NULL) {
                sub = malloc(sizeof(char) * 2);
                sub[0] = character;
                sub[1] = '\0';
            } else {
                sub = cyml_utils_append_character(sub, character);
            }

            continue;
        }

        if(character == '\\') {
            skip = 1;
            continue;
        }

        if(0 == scope && (character == ' ' || character == '\0')) {
            if (sub != NULL) {
                CYMLToken token = cyml_parse_token(sub);

                if(token.type != TOKEN_UNDEFINED) {
                    if (result.tokens == NULL)
                        result.tokens = malloc(sizeof(CYMLToken));
                    else
                        result.tokens = realloc(result.tokens, sizeof(CYMLToken) * (result.tokenCount + 1));

                    result.tokens[result.tokenCount] = token;

                    ++result.tokenCount;
                } else {
                    return cyml_error_token_result();
                }

                if(sub != NULL) {
                    free(sub);
                    sub = NULL;
                }
            }
        } else {
            if(character == '"')
                scope = !scope;

            if(sub == NULL) {
                sub = malloc(sizeof(char) * 2);
                sub[0] = character;
                sub[1] = '\0';
            } else
                sub = cyml_utils_append_character(sub, character);
        }
    }

    if(sub != NULL) {
        // Unexpected end of string
        return cyml_error_token_result();
    }

    return result;
}
