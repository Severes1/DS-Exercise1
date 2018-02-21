#include <mqueue.h>
#include "messages.h"
#include <pthread.h>
#include "treemap.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_THREADS 32
#define TRUE 1
#define FALSE 0

Response RESPONSE_SUCCESS;
Response RESPONSE_ERROR;

pthread_mutex_t mutex_cpy_msg;
pthread_cond_t cond_cpy_msg;
int msg_copied;

TreeMap database = NULL; 
pthread_mutex_t mutex_database;

/* Create a fresh empty database */
Response process_init(Request request) {
    assert(request->status == 0);
    pthread_mutex_lock(&mutex_database);
    if (database != NULL) {
        treemap_free(database);
    }
    database = treemap_create();
    pthread_mutex_unlock(&mutex_database);
    return RESPONSE_SUCCESS;
}

/* Add a new key to the database */
Response process_set(Request request) {
    assert(request->status == 1);
    Response ret;
    pthread_mutex_lock(&mutex_database);
    if (database == NULL) {
        ret = RESPONSE_ERROR;
    } else if (treemap_contains(database, request->key)) {
        ret = RESPONSE_ERROR;
    } else if (treemap_set(database,
                request->key, 
                request->value1,
                request->value2) != 0) {
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

    char * value1 = malloc(MAX_STRING_SIZE);
    float * value2 = malloc(sizeof(float));

    if (treemap_get(database, request->key, value1, value2) == 0) {
        ret = generate_response(RESPONSE_SUCCESS_FLAG, request->key, value1, *value2);
    } else {
        ret = RESPONSE_ERROR;
    }

    free(value1);
    free(value2);
    return ret;
}

/* Modify an existing value in the database */
Response process_modify(Request request) {
    assert(request->status == 3);
    Response ret;
    pthread_mutex_lock(&mutex_database);

    if (!treemap_contains(database, request->key)) {
        ret = RESPONSE_ERROR;
    } else if (treemap_set(database,
            request->key,
            request->value1, 
            request->value2) == 0) {
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
    
    if (treemap_delete(database, request->key) != 0) {
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
    int count = treemap_count(database); 
    ret = generate_response(RESPONSE_SUCCESS_FLAG, count, NULL, 0.0f);
    return ret;
}

void * process_request(void * buffer) {
    pthread_mutex_lock(&mutex_cpy_msg);

    Request request = (Request) buffer;
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

    pthread_t worker_threads[MAX_THREADS];

    /* Maybe use a ready_queue */
    int thread_status[MAX_THREADS]; // 0 available, 1 busy
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_status[i] = 0;
    }

    pthread_attr_t thread_attr; 
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    int last_index = 0;
    int exiting = 0;
    while(!exiting) {
        printf("Awaiting messages...\n");
        if (receive_message(incoming, buffer) < 0) {
            perror("Failed to receive message");
            exit(1);
        }
        printf("Received message: \n%s\n", message_to_string(buffer));
        pthread_t * new_pthread = NULL;
        for (int j = 0; j < MAX_THREADS; j++) {
            int index = (j + last_index) % MAX_THREADS;
            if (thread_status[index] == 0) {
                new_pthread = &worker_threads[index];
                last_index = index;
                thread_status[index] = 1; // busy
                break;
            }
        }
        
        msg_copied = FALSE;
        pthread_mutex_lock(&mutex_cpy_msg);
        /*pthread_create(new_pthread, &thread_attr, process_request, buffer);
        while (!msg_copied) {
            pthread_cond_wait(&cond_cpy_msg, &mutex_cpy_msg);
        }*/

        pthread_mutex_unlock(&mutex_cpy_msg);
        process_request(buffer);
    } 
    
    remove_connection(QUEUE_NAME);
}
