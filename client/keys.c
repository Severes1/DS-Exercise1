#include <mqueue.h>
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int send_request(const Request request, Response response) {
    Connection server = open_connection_write("/SERVER");
    Connection response_queue = create_connection_read("/CLIENT");
    send_message(server, request);
    receive_message(response_queue, response);
    return response->status;
}

/*
 * This call allows the initialization of the system. Through this service,
 * all triples that were stored are destroyed.
 * The function returns 0 on success and -1 on error.
*/
int init(void) {
    Request request = generate_request(0, 0, NULL, 0.0f, "/CLIENT");
    Message response;
    send_request(request, &response);
    return response.status;
}

/*
 * This service inserts the element <key, value1, value2>. 
 * The service returns 0 if it was inserted successfully and -1 in case of error.
 * It is considered an error to try to insert a key that already exists.
 * */
int set_value(int key, char * value1, float value2) {
    Request request = generate_request(1, key, value1, value2, "/CLIENT");
    Message response;
    send_request(request, &response);
    return response.status;
}

/*
 * This service allows you to obtain the values associated with the key. 
 * The values are returned in value1 and value2.
 * The function returns 0 on success and -1 on error, for example,
 * if there is no element with that key.
*/
int get_value(int key, char * value1, float * value2) {
    Request request = generate_request(2, key, NULL, 0.0f, "/CLIENT");
    Message response;
    send_request(request, &response);
    memcpy(value1, response.value1, 256);
    *value2 = response.value2;
    return response.status;
}

/*
 * This service allows modifying the values associated with a key. 
 * The function returns 0 on success and -1 on error, for example, 
 * if there is no element with that key.
*/ 
int modify_value(int key, char * value1, float * value2) {
    Request request = generate_request(3, key, value1, *value2, "/CLIENT");
    Message response;
    send_request(request, &response);
    return response.status;
}

/*
 * This service allows you to delete the element whose key is key. 
 * The function returns 0 on success and -1 on error. In case the key 
 * does not exist, -1 is also returned.
*/
int delete_key(int key) {
    Request request = generate_request(4, key, NULL, 0.0f, "/CLIENT");
    Message response;
    send_request(request, &response);
    return response.status;
}

/*
 * This service returns the number of items stored on the server. 
 * The call returns -1 in case of error
*/
int num_items() {
    Request request = generate_request(5, 0, NULL, 0.0f, "/CLIENT");
    Message response;
    send_request(request, &response);
    if (response.status == -1) {
        return -1;
    }
    return response.key;
}
