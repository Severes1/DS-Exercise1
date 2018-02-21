#include "treemap.h"
#include "treemap_tests.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

int treemap_contains_test() {
    TreeMap tm = treemap_create();

    treemap_set(tm, 0, "Hello World", 5.4f);

    assert( treemap_contains(tm, 0) == TRUE  );
    assert( treemap_contains(tm, 1) == FALSE );

    treemap_set(tm, 1, "Goodbye", 1.2f);

    assert( treemap_contains(tm, 0) == TRUE  );
    assert( treemap_contains(tm, 1) == TRUE  );

    return 0;
}

int treemap_get_test() {
    TreeMap tm = treemap_create();
    treemap_set(tm, 0, "team", 100.232f);
    char * res1 = malloc(MAX_STRING_SIZE);
    float * res2 = malloc(sizeof(float));
    treemap_get(tm, 0, res1, res2);
    assert( strcmp(res1, "team") == 0 );
    assert( *res2 = 100.232f );
    return 0;
}

int treemap_set_test() {
    TreeMap tm = treemap_create();
    treemap_set(tm, 10, "George", -0.124124f);
    treemap_set(tm, -2, "oops", 0.0f);
    char * res1 = malloc(MAX_STRING_SIZE);
    float * res2 = malloc(sizeof(float));
    
    treemap_get(tm, 10, res1, res2);
    assert( strcmp(res1, "George") == 0 );
    assert( *res2 == -0.124124f);
    
    treemap_get(tm, -2, res1, res2);
    assert( strcmp(res1, "oops") == 0 );
    assert( *res2 == 0.0f);

    // Modify the leaf entry
    treemap_set(tm, -2, "Paul", -2200.1f);

    treemap_get(tm, -2, res1, res2);
    assert( strcmp(res1, "Paul") == 0 );
    assert( *res2 == -2200.1f);

    // Modify the root entry
    treemap_set(tm, 10, "Ringo", 1.0f);

    treemap_get(tm, 10, res1, res2);
    assert( strcmp(res1, "Ringo") == 0 );
    assert( *res2 == 1.0f);

    return 0;
}

int treemap_delete_test() {
    TreeMap tm = treemap_create();
    treemap_set(tm, -5, "rolly", 8.4f);

    assert( treemap_contains(tm, -5) );
    treemap_set(tm, 200, "ok", 10.4f);

    // Last element is an edge case.
    treemap_delete(tm, -5);
    assert( !treemap_contains(tm, -5));
    assert( treemap_count(tm) == 1 );        

    assert( treemap_contains(tm, 200) );

    treemap_delete(tm, 200);

    assert( !treemap_contains(tm, 200) );

    return 0;
}
int treemap_count_test() {
    TreeMap tm = treemap_create();
    assert( treemap_count(tm) == 0 );
    treemap_set(tm, 0, "", 0.0f);
    assert( treemap_count(tm) == 1 );

    treemap_set(tm, 1, "1", 1.0f);

    assert( treemap_count(tm) == 2);

    treemap_delete(tm, 1);

    assert( treemap_count(tm) == 1);
   
    return 0; 
}

int main() {
    int tests_run = 0;
    treemap_contains_test();
    tests_run++;
    
    treemap_count_test();
    tests_run++;

    treemap_get_test();
    tests_run++;

    treemap_set_test(); 
    tests_run++;

    treemap_delete_test();
    tests_run++;
    


    printf("All %d tests passed!\n", tests_run);
}
