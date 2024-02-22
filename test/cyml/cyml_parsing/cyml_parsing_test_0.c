#include "test_shared.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("U32 label 100");

        ensure(result.code == CYML_OK);

        ensure(result.tokenCount == 3);
        ensure(result.tokens != NULL);

        ensure(result.tokens[0].type == TOKEN_TYPE);
        ensure(result.tokens[1].type == TOKEN_WORD);
        ensure(result.tokens[2].type == TOKEN_UNSIGNED_INT);

        ensure(*((CYMLType*) result.tokens[0].value) == TYPE_UNSIGNED_INT)
        ensure(strcmp("label", result.tokens[1].value) == 0);
        ensure(*((unsigned int*) result.tokens[2].value) == 100)

        cyml_free_token_result(result);
    }

    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("FLOAT label 0.9");

        ensure(result.code == CYML_OK);

        ensure(result.tokenCount == 3);
        ensure(result.tokens != NULL);

        ensure(result.tokens[0].type == TOKEN_TYPE);
        ensure(result.tokens[1].type == TOKEN_WORD);
        ensure(result.tokens[2].type == TOKEN_FLOAT);

        ensure(*((CYMLType*) result.tokens[0].value) == TYPE_FLOAT)
        ensure(strcmp("label", result.tokens[1].value) == 0);
        ensure(*((float*) result.tokens[2].value) == 0.9f)

        cyml_free_token_result(result);
    }

    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("FLOAT label -15.69");

        ensure(result.code == CYML_OK);

        ensure(result.tokenCount == 3);
        ensure(result.tokens != NULL);

        ensure(result.tokens[0].type == TOKEN_TYPE);
        ensure(result.tokens[1].type == TOKEN_WORD);
        ensure(result.tokens[2].type == TOKEN_FLOAT);

        ensure(*((CYMLType*) result.tokens[0].value) == TYPE_FLOAT)
        ensure(strcmp("label", result.tokens[1].value) == 0);
        ensure(*((float*) result.tokens[2].value) == -15.69f)

        cyml_free_token_result(result);
    }

    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("STRING label \"Hello world !\"");

        ensure(result.code == CYML_OK);

        ensure(result.tokenCount == 3);
        ensure(result.tokens != NULL);

        ensure(result.tokens[0].type == TOKEN_TYPE);
        ensure(result.tokens[1].type == TOKEN_WORD);
        ensure(result.tokens[2].type == TOKEN_STRING);

        ensure(*((CYMLType*) result.tokens[0].value) == TYPE_STRING)
        ensure(strcmp("label", result.tokens[1].value) == 0);
        ensure(strcmp("Hello world !", result.tokens[2].value) == 0);

        cyml_free_token_result(result);
    }

    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("STRING label \"Hello \\\" world !\"");

        ensure(result.code == CYML_OK);

        ensure(result.tokenCount == 3);
        ensure(result.tokens != NULL);

        ensure(result.tokens[0].type == TOKEN_TYPE);
        ensure(result.tokens[1].type == TOKEN_WORD);
        ensure(result.tokens[2].type == TOKEN_STRING);

        ensure(*((CYMLType*) result.tokens[0].value) == TYPE_STRING)
        ensure(strcmp("label", result.tokens[1].value) == 0);
        ensure(strcmp("Hello \" world !", result.tokens[2].value) == 0);

        cyml_free_token_result(result);
    }

    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("FLOAT label -15.69 \"");

        ensure(result.code == CYML_ERROR);

        cyml_free_token_result(result);
    }

    TEST_CASE {
        CYMLTokenResult result = cyml_tokens_parse_string("");

        ensure(result.code == CYML_OK);

        ensure(result.tokenCount == 0);
        ensure(result.tokens == NULL);

        cyml_free_token_result(result);
    }

    return 0;
}