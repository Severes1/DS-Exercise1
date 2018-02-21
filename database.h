typedef int Database;

Database db_init(int data_size);
int db_contains(Database db, int key);
int db_insert(Database db, int key, char* data);
int db_update(Database db, int key, char* data);
int db_get(Database db, int key, char* data);
int db_delete(Database db, int key);
int db_count(Database db);
int db_close(Database db);
int db_destroy(Database db);
