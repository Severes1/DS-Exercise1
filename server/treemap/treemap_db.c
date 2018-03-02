#include "database.h"
#include "treemap.h"
#include <stdlib.h>

struct Database {
    TreeMap treemap;
};

Database db_init(int data_size) {
    Database db = malloc(sizeof(struct Database));
    db->treemap = treemap_create(data_size);
    return db;
}

int db_contains(Database db, int key) {
    return treemap_contains(db->treemap, key);
}

int db_insert(Database db, int key, const char* data) {
    if (treemap_contains(db->treemap, key)) {
        return -1;
    } else {
        return treemap_set(db->treemap, key, data);
    }
}

int db_update(Database db, int key, const char* data) {
    if (!treemap_contains(db->treemap, key)) {
        return -1;
    } else {
        return treemap_set(db->treemap, key, data);
    }
}

int db_get(Database db, int key, char* data) {
    return treemap_get(db->treemap, key, data);
}

int db_delete(Database db, int key) {
    return treemap_delete(db->treemap, key);
}

int db_count(Database db) {
    return treemap_count(db->treemap);
}

// TODO maybe save it in file
int db_close(Database db) {
    treemap_free(db->treemap); 
    return 0;
}

int db_destroy(Database db) {
    treemap_free(db->treemap);
    return 0;
}
