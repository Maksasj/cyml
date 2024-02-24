#ifndef _CYML_H_
#define _CYML_H_

typedef enum CYMLTokenType {
    TOKEN_UNDEFINED,

    TOKEN_TYPE,
    TOKEN_WORD,

    TOKEN_BEGIN,
    TOKEN_END,

    TOKEN_STRING,
    TOKEN_UNSIGNED_INT,
    TOKEN_FLOAT
} CYMLTokenType;

typedef struct CYMLToken {
    CYMLTokenType type;
    void* value;
} CYMLToken;

typedef enum CYMLType {
    TYPE_UNDEFINED, // Used for errors

    TYPE_TUPLE,
    TYPE_STRING,
    TYPE_FLOAT,
    TYPE_UNSIGNED_INT
} CYMLType;

typedef enum CYMLResult {
    CYML_OK,
    CYML_ERROR
} CYMLResult;

typedef struct CYMLTokenResult {
    CYMLToken* tokens;
    unsigned int tokenCount;

    CYMLResult code;
} CYMLTokenResult;

int cyml_utils_string_contains_character(const char* string, char character);
char* cyml_utils_append_character(char* string, char character);

int cyml_utils_is_u32(const char* str);
unsigned int cyml_utils_string_to_u32(const char* str);

int cyml_utils_is_float(const char* str);
float cyml_utils_string_to_float(const char* str);

void cyml_free_token_result(CYMLTokenResult result);

CYMLTokenResult cyml_empty_token_result(CYMLResult code);
CYMLTokenResult cyml_error_token_result();

CYMLTokenResult cyml_tokens_parse_string(const char* string);

typedef struct CYMLNode {
    CYMLType type;

    unsigned int dataSize;
    void* data;

    char* label;
} CYMLNode;

CYMLNode* cyml_create_node(CYMLType type, const char* label);

CYMLNode* cyml_append_string_node(CYMLNode* node, const char* label, const char* value);
CYMLNode* cyml_append_float_node(CYMLNode* node, const char* label, float value);
CYMLNode* cyml_append_number_node(CYMLNode* node, const char* label, unsigned int value);
CYMLNode* cyml_append_tuple_node(CYMLNode* node, const char* label);

CYMLNode* cyml_parse_string(const char* string);

char* cyml_stringify_node(CYMLNode* node);

void cyml_free_node(CYMLNode* node);

#endif