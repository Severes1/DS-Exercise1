#include "database.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "check.h"

#define TRUE 1
#define FALSE 0

struct Database {
    char * directory_name;
    int data_size;
    int count;
};
char * get_filename(Database db, int key) {
    char * ret = malloc(50);
    sprintf(ret, "%s/%d", db->directory_name, key);
    return ret;
}
Database db_init(int data_size) {
    system("rm db/*");
    system("mkdir db");
    Database db = malloc(sizeof(struct Database));
    db->directory_name = "db";
    db->data_size = data_size;
    db->count = 0;
    return db;
}
int db_contains(Database db, int key) {
    if( access( get_filename(db, key), F_OK ) != -1 ) {
        return TRUE;
     } else {
        return FALSE;
     }
}
int write_file(Database db, int key, const char* data) {
    FILE *fp = fopen(get_filename(db, key), "w");
    CHECK(fwrite(data, db->data_size, 1, fp), return -1);
    fclose(fp);
    return 0;
}

int db_insert(Database db, int key, const char* data) {
    if (db_contains(db, key)) {
        return -1;
    }
    
    CHECK(write_file(db, key, data), return -1);
    db->count++;
    return 0;
}
int db_update(Database db, int key, const char* data) {
    if (!db_contains(db, key)) {
        return -1;
    }
    return write_file(db, key, data);
}
int db_get(Database db, int key, char* data) {
    if (!db_contains(db, key)) {
        return -1;
    }
    FILE *fp = fopen(get_filename(db, key), "r");
    CHECK(fread(data, db->data_size, 1, fp), return -1);
    fclose(fp);
    return 0;
}
int db_delete(Database db, int key) {
    if (!db_contains(db, key)) {
        return -1;
    }

    char * str = malloc(50);
    sprintf(str, "rm %s", get_filename(db, key));
    system(str);

    db->count--;
    return 0;
}
int db_count(Database db) {
    return db->count;
}
int db_close(Database db) {
    return 0;
}
int db_destroy(Database db) {
    char * str = malloc(50);
    sprintf(str, "rm %s", db->directory_name);
    system(str);
    return 0;
}
