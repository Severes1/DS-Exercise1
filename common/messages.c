#include <mqueue.h>
#include "messages.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

struct Connection {
    int id;
    char name[MAX_RETURN_QUEUE_NAME];
}; 

typedef struct Connection ConnectionStruct;

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
    mq_unlink(name);
    struct mq_attr mqueue_attr;
    mqueue_attr.mq_maxmsg = MAX_MSG;
    mqueue_attr.mq_msgsize = sizeof(Message);
    mqd_t mq_id = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0666, &mqueue_attr);
    if (mq_id == -1) {
        perror("open_connection_read");
    }
    mq_getattr(mq_id, &mqueue_attr);
    assert(mq_id != -1);
    Connection ret = malloc(sizeof(ConnectionStruct));
    ret->id = mq_id;
    memcpy(ret->name, name, MAX_RETURN_QUEUE_NAME);
    return ret;
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
    //printf("Connection name: \"%s\"\n", name);
    mq_getattr(mq_id, &mqueue_attr);
    assert(mq_id != -1);
    Connection ret = malloc(sizeof(ConnectionStruct));
    ret->id = mq_id;
    memcpy(ret->name, name, MAX_RETURN_QUEUE_NAME);
    return ret;
}

void close_connection(Connection connection) {
    mq_close(connection->id);
}

void remove_connection(char * name) {
    mq_unlink(name);
}

int send_message(Connection connection, Message * message) {
    printf("Sending to %s: \n%s\n", connection->name, message_to_string(message));
    int ret = mq_send(connection->id, (char *) message, sizeof(Message), 0);
    if (ret < 0) {
        perror("send_message");
    }
    return ret;
}

int receive_message(Connection connection, Message * buffer) {
    int ret = mq_receive(connection->id, (char *) buffer, MSG_SIZE, NULL);
    printf("Received message: \n%s\n", message_to_string(buffer));
    return ret;
}

char * message_to_string(Message * msg) {
    char * ret = malloc(MSG_SIZE);
    if (msg->status == 0 && msg->key == 0) {
	    if (strlen(msg->return_queue) > 0) {
            sprintf(ret, "\tINIT");	
	        return ret;
        } else {
	        sprintf(ret, "\tSUCCESS");
	        return ret;
	    }
    } 
    if (msg->status == 1) {
        sprintf(ret, "\tSET key:%d, \n\tvalue1: %s, \n\tvalue2: %f, \n\treturn_queue: %s",
		      msg->key,
		      msg->value1,
 		      msg->value2,
    	      msg->return_queue);
        return ret;
    }
    if (msg->status == 2) {
        sprintf(ret, "\tGET key: %d", msg->key);	
	return ret;
    }
    if (msg->status == 3) {
        sprintf(ret, "\tMODIFY key:%d, \n\tvalue1: %s, \n\tvalue2: %f, \n\treturn_queue: %s",
		      msg->key,
		      msg->value1,
 		      msg->value2,
    		  msg->return_queue);
        return ret;
    }
    if (msg->status == 4) {
        sprintf(ret, "\tDELETE key: %d", msg->key);	
	return ret;
    }
    if (msg->status == 5) {
	sprintf(ret, "\tNUM_ITEMS");
	return ret;
    }
    if(msg->status == -1) {
	sprintf(ret, "\tERROR");	
	return ret;
    }
    sprintf(ret,
            "\tstatus: %d, \n\tkey: %d, \n\tvalue1: %s, \n\tvalue2: %f, \n\treturn_queue: %s",
            msg->status,
            msg->key,
            msg->value1,
            msg->value2,
            msg->return_queue);
    return ret;
}
