#include "treemap.h"
#include "treemap_tests.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

typedef struct data {
    char value1[256];
    float value2;
} Data;

Data new_data(char * value1, float value2) {
    Data data;
    memcpy(data.value1, value1, 256); 
    data.value2 = value2;
    return data;
}

int treemap_contains_test() {
    TreeMap tm = treemap_create(sizeof(Data));
    Data data1 = new_data("Hello world", 5.4f);

    treemap_set(tm, 0, (char *) &data1);

    assert( treemap_contains(tm, 0) == TRUE  );
    assert( treemap_contains(tm, 1) == FALSE );

    Data data2 = new_data("Goodbye", 1.2f);
    treemap_set(tm, 1, (char *) &data2);

    assert( treemap_contains(tm, 0) == TRUE  );
    assert( treemap_contains(tm, 1) == TRUE  );

    return 0;
}

int treemap_get_test() {
    TreeMap tm = treemap_create(sizeof(Data));
    
    Data data1 = new_data("team", 100.232f);
    treemap_set(tm, 0, (char *) &data1);

    Data data2;
    treemap_get(tm, 0, (char *) &data2);
    assert( strcmp(data2.value1, "team") == 0 );
    assert( data2.value2 = 100.232f );
    return 0;
}

int treemap_set_test() {
    TreeMap tm = treemap_create(sizeof(Data));
    Data data = new_data("George", -0.124124f);
    treemap_set(tm, 10, (char *) &data);

    data = new_data("oops", 0.0f);
    treemap_set(tm, -2, (char *) &data);

    Data res;
    treemap_get(tm, 10, (char *) &res);
    
    assert( strcmp(res.value1, "George") == 0 );
    assert( res.value2 == -0.124124f);
    
    treemap_get(tm, -2, (char *) &res);
    assert( strcmp(res.value1, "oops") == 0 );
    assert( res.value2 == 0.0f);

    // Modify the leaf entry
    data = new_data("Paul", -2200.1f);
    treemap_set(tm, -2, (char *) &data);

    treemap_get(tm, -2, (char *) &res);
    assert( strcmp(res.value1, "Paul") == 0 );
    assert( res.value2 == -2200.1f);

    // Modify the root entry
    data = new_data("Ringo", 1.0f);
    treemap_set(tm, 10, (char *) &data);

    treemap_get(tm, 10, (char *) &res);
    assert( strcmp(res.value1, "Ringo") == 0 );
    assert( res.value2 == 1.0f);

    return 0;
}

int treemap_delete_test() {
    TreeMap tm = treemap_create(sizeof(Data));
    Data data = new_data("rolly", 8.4f);
    treemap_set(tm, -5, (char *) &data);

    assert( treemap_contains(tm, -5) );

    data = new_data("ok", 10.4f);
    treemap_set(tm, 200, (char *) &data);

    // Last element is an edge case.
    treemap_delete(tm, -5);
    assert( !treemap_contains(tm, -5));
    assert( treemap_count(tm) == 1 );        

    assert( treemap_contains(tm, 200) );

    treemap_delete(tm, 200);

    assert( !treemap_contains(tm, 200) );
    // TODO test more complicated tree with patching
    return 0;
}

int treemap_count_test() {
    TreeMap tm = treemap_create(sizeof(Data));
    assert( treemap_count(tm) == 0 );
    Data data = new_data("", 0.0f);
    
    treemap_set(tm, 0, (char *) &data);
    assert( treemap_count(tm) == 1 );

    treemap_set(tm, 1, (char *) &data);

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
