#include <mqueue.h>
#include "messages.h"
#include <pthread.h>
#include "database.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Data Data;

Data new_data(char * value1, float value2); 

/* Create a fresh empty database */
Response process_init(Request request);

/* Add a new key to the database */
Response process_set(Request request);

/* Retrieve a key from the database */
Response process_get(Request request);

/* Modify an existing value in the database */
Response process_modify(Request request);

/* Delete a given key from the database */
Response process_delete(Request request);

/* Return a message with the number of entries in the database */
Response process_count(Request request);

/* The entry point for a worker thread. Copies the buffer using a mutex, and sends a response */
void * process_request(void * buffer);
