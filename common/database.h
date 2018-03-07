/*
 *  The interface for any module that wants to act as a database. 
 *
 */

/* Abstract type to be defined by implementation */
typedef struct Database *Database;

/* Initialize the database
 * int data_size: the size of each piece of data that can be stored in this database. */
Database db_init(int data_size);

/*  Returns whether the given key exists in the given database
 *  Returns 1 if it exists, 0 if it doesn't, and -1 on error. */ 
int db_contains(Database db, int key);

/*
 *  Insert the given data into the database, and associate it with the given key.
 *  Fails if the key already exists. Returns -1 on failure, 0 on success. */
int db_insert(Database db, int key, const char* data);

/* Replace the existing data associated with the given key with the given data
 * Fails if the key does not exist. Returns 0 on success, and -1 on failure. */
int db_update(Database db, int key, const char* data);

/* Retrieve the data associated with the given key, and store it in data
 * Returns -1 if the key doesn't exist. */
int db_get(Database db, int key, char* data);

/* Remove the given key and its associated data from the database. */
int db_delete(Database db, int key);

/* Return the number of keys stored in the database  */
int db_count(Database db);

/* Indicates that you no longer need to use this database */
int db_close(Database db);

/* Removes all data from the database, and closes it. */
int db_destroy(Database db);
