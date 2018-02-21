#include <mqueue.h>
#include "messages.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

void generate_message(Message * message,
        char status, int key, char * value1, float value2, char * return_queue) {

    message->status = status;
    message->key = key;
    if (value1 != NULL) {
        memcpy(message->value1, value1, MAX_STRING_SIZE);
    }
    message->value2 = value2;
    if (return_queue != NULL) {
        memcpy(message->return_queue, return_queue, MAX_RETURN_QUEUE_NAME);
    }
}

Request generate_request(char status, int key, char * value1, float value2, char * return_queue) {
    Request request = malloc(sizeof(Message));
    generate_message(request, status, key, value1, value2, return_queue);
    return request;
}

Response generate_response(char status, int key, char * value1, float value2) {
    Response response = malloc(sizeof(Message));
    generate_message(response, status, key, value1, value2, NULL);
    return response;
}

Connection create_connection_read(char *name) {
    struct mq_attr mqueue_attr;
    mq_unlink(name);
    mqueue_attr.mq_maxmsg = MAX_MSG;
    mqueue_attr.mq_msgsize = sizeof(Message);
    mqd_t mq_id = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0666, &mqueue_attr);
    if (mq_id == -1) {
        perror("open_connection_read");
    }
    mq_getattr(mq_id, &mqueue_attr);
    assert(mq_id != -1);
    return mq_id;
}   


/* Connection open_connection_read(char * name) {
    struct mq_attr mqueue_attr;
    mqueue_attr.mq_maxmsg = MAX_MSG;
    mqueue_attr.mq_msgsize = MSG_SIZE;
    mqd_t mq_id = mq_open(QUEUE_NAME, O_RDONLY, 0662, mqueue_attr);
    if (mq_id == -1) {
        perror("open_connection_read");
    }
    assert(mq_id != -1);
    return mq_id;
} */

Connection open_connection_write(char * name) {
    struct mq_attr mqueue_attr;
    mqueue_attr.mq_maxmsg = MAX_MSG;
    mqueue_attr.mq_msgsize = sizeof(Message);
    mqd_t mq_id = mq_open(QUEUE_NAME, O_WRONLY, 0666, &mqueue_attr);
    if (mq_id == -1) {
        perror("open_connection_write");
    }
    printf("Connection name: \"%s\"\n", name);
    mq_getattr(mq_id, &mqueue_attr);
    assert(mq_id != -1);
    return mq_id;
}

void close_connection(Connection connection) {
    mq_close(connection);
}

void remove_connection(char * name) {
    mq_unlink(name);
}

int send_message(Connection connection, Message * message) {
    printf("Sending: %s\n", message_to_string(message));
    printf("Connection: %d\n", connection);
    int ret = mq_send(connection, (char *) message, sizeof(Message), 0);
    if (ret < 0) {
        perror("send_message");
    }
    printf("mq_send returned %d\n", ret);
    return ret;
}

int receive_message(Connection connection, Message * buffer) {
    return mq_receive(connection, (char *) buffer, MSG_SIZE, NULL);
}

char * message_to_string(Message * msg) {
    char * ret = malloc(MSG_SIZE);
    sprintf(ret,
            "status: %d, key: %d, value1: %s, value2: %f, return_queue: %s",
            msg->status,
            msg->key,
            msg->value1,
            msg->value2,
            msg->return_queue);
    return ret;
}
