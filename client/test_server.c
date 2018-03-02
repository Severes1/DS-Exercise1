#include "keys.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
    init();
    set_value(42, "Hello world", 12.3f);
    set_value(-2, "Goodbye", -88.2f);

    char ret1[256];
    float ret2;
    get_value(42, ret1, &ret2);
    assert(strcmp(ret1, "Hello world") == 0);
    assert(ret2 == 12.3f);
    
    get_value(42, ret1, &ret2);
    assert(strcmp(ret1, "Hello world") == 0);
    assert(ret2 == 12.3f);

    assert ( num_items() == 2 );

    assert ( delete_key(1) == -1 );
    
    assert ( delete_key(42) == 0 );

    assert ( num_items() == 1 );

    printf("Test passed!\n");
}
