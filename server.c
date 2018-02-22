#include <mqueue.h>
#include "messages.h"
#include <pthread.h>
#include "database.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_THREADS 32
#define TRUE 1
#define FALSE 0

Response RESPONSE_SUCCESS;
Response RESPONSE_ERROR;

pthread_mutex_t mutex_cpy_msg;
pthread_cond_t cond_cpy_msg;
int msg_copied;

Database database = NULL; 
pthread_mutex_t mutex_database;

typedef struct Data {
    char value1[256];
    float value2; 
} Data;

Data new_data(char * value1, float value2) {
    Data data;
    memcpy(data.value1, value1, 256);
    data.value2 = value2;
    return data;
}

/* Create a fresh empty database */
Response process_init(Request request) {
    assert(request->status == 0);
    pthread_mutex_lock(&mutex_database);
    if (database != NULL) {
        db_destroy(database);
    }
    database = db_init(sizeof(Data));
    pthread_mutex_unlock(&mutex_database);
    return RESPONSE_SUCCESS;
}

/* Add a new key to the database */
Response process_set(Request request) {
    assert(request->status == 1);
    Response ret;
    Data data = new_data(request->value1, request->value2);
    pthread_mutex_lock(&mutex_database);
    if (database == NULL) {
        ret = RESPONSE_ERROR;
    } else if (db_contains(database, request->key)) {
        ret = RESPONSE_ERROR;
    } else if (db_insert(database, request->key, (char *) &data) != 0) {
        ret = RESPONSE_ERROR;
    } else {
        ret = RESPONSE_SUCCESS;
    }
    pthread_mutex_unlock(&mutex_database);
    return ret;
}

/* Retrieve a key from the database */
Response process_get(Request request) {
    assert(request->status == 2);
    // No need to request mutex for reading.
    Response ret;

    Data res;
    if (db_get(database, request->key, (char *) &res) == 0) {
        ret = generate_response(RESPONSE_SUCCESS_FLAG, request->key,
                res.value1, res.value2);
    } else {
        ret = RESPONSE_ERROR;
    }

    return ret;
}

/* Modify an existing value in the database */
Response process_modify(Request request) {
    assert(request->status == 3);
    Response ret;
    
    Data data = new_data(request->value1, request->value2);
    pthread_mutex_lock(&mutex_database);

    if (!db_contains(database, request->key)) {
        ret = RESPONSE_ERROR;
    } else if (db_update(database, request->key, (char *) &data) == 0) {
        ret = RESPONSE_SUCCESS;
    } else {
        ret = RESPONSE_ERROR;
    }

    pthread_mutex_unlock(&mutex_database);
    return ret;
}

/* Delete a given key from the database */
Response process_delete(Request request) {
    assert(request->status == 4);
    Response ret;
    pthread_mutex_lock(&mutex_database);
    
    if (db_delete(database, request->key) != 0) {
        ret = RESPONSE_ERROR;
    } else {
        ret = RESPONSE_SUCCESS;
    }

    pthread_mutex_unlock(&mutex_database);
    return ret;
}

/* Return a message with the number of entries in the database */
Response process_count(Request request) {
    assert(request->status == 5);
    Response ret;
    int count = db_count(database); 
    ret = generate_response(RESPONSE_SUCCESS_FLAG, count, NULL, 0.0f);
    return ret;
}

void * process_request(void * buffer) {
    printf("process_request %s\n", message_to_string(buffer));
    pthread_mutex_lock(&mutex_cpy_msg);

    Request request = malloc(sizeof(Message));
    memcpy(request, buffer, sizeof(Message));

    msg_copied = TRUE;

    pthread_cond_signal(&cond_cpy_msg);
    pthread_mutex_unlock(&mutex_cpy_msg); 

    Response response;

    switch(request->status) {
        case 0:
            response = process_init(request);
            break;
        case 1:
            response = process_set(request);
            break;
        case 2:
            response = process_get(request);
            break;
        case 3:
            response = process_modify(request);
            break;
        case 4:
            response = process_delete(request);
            break;
        case 5:
            response = process_count(request);
            break;
        default:
            response = RESPONSE_ERROR;
            break;
    }

    // Send response message to response queue
    Connection outgoing = open_connection_write(request->return_queue); 
    send_message(outgoing, response);
    close_connection(outgoing);
    return NULL;
}

int main() {
    Connection incoming = create_connection_read(QUEUE_NAME);
    RESPONSE_SUCCESS = generate_response(0, 0, NULL, 0.0f);
    RESPONSE_ERROR = generate_response(1, 0, NULL, 0.0f);

    Request buffer = malloc(MSG_SIZE + 1);

    pthread_attr_t thread_attr; 
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    int exiting = 0;
    while(!exiting) {
        printf("Awaiting messages...\n");
        if (receive_message(incoming, buffer) < 0) {
            perror("Failed to receive message");
            exit(1);
        }
        printf("Received message: \n%s\n", message_to_string(buffer));
        pthread_t new_pthread;
       
        msg_copied = FALSE;
        pthread_mutex_lock(&mutex_cpy_msg);
        pthread_create(&new_pthread, &thread_attr, process_request, buffer);
        while (!msg_copied) {
            pthread_cond_wait(&cond_cpy_msg, &mutex_cpy_msg);
        }

        pthread_mutex_unlock(&mutex_cpy_msg);
    } 
    
    remove_connection(QUEUE_NAME);
}
