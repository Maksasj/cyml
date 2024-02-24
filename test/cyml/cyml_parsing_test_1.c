#include "test_shared.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    TEST_CASE {
        CYMLNode* parse = cyml_parse_string("U32 label 100");

    }

    return 0;
}