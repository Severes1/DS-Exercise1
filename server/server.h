#include <mqueue.h>
#include "messages.h"
#include <pthread.h>
#include "database.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Response RESPONSE_SUCCESS;
Response RESPONSE_ERROR;

pthread_mutex_t mutex_cpy_msg;
pthread_cond_t cond_cpy_msg;
int msg_copied;

Database database = NULL; 
pthread_mutex_t mutex_database;

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

void * process_request(void * buffer);
