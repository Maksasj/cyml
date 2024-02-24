#include "test_shared.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    TEST_CASE {
        CYMLNode* node = cyml_create_node(TYPE_TUPLE, "testNode");

        cyml_append_string_node(node, "label", "Hello world !");
        cyml_append_string_node(node, "anotherNode", "But another hello world !");

        const char* string = cyml_stringify_node(node);
        printf("%s", string);
        free(string);

        // cyml_free_node(node);

        // cyml_append_float_node(node, "floatField", 15.9f);
        // cyml_append_number_node(node, "number", 9);
//
        // CYMLNode* child = cyml_append_tuple_node(node, "tupleNode");
//
        // cyml_append_float_node(child, "floatField", 15.9f);
        // cyml_append_number_node(child, "number", 9);
    }

    return 0;
}