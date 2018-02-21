#include <mqueue.h>
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>

#define CLIENT_QUEUE "/CLIENT"

int main() {

    mq_unlink(CLIENT_QUEUE);

    struct mq_attr mqueue_attr;
    mqueue_attr.mq_maxmsg = MAX_MSG;
    mqueue_attr.mq_msgsize = sizeof(Message);

    Connection server = open_connection_write("/SERVER");//mq_open("/SERVER", O_WRONLY, 0666, &mqueue_attr); 
    Connection response_queue = create_connection_read("/CLIENT");

    if (response_queue < 0) {
        perror("response_queue");
    } else {
        printf("reponse_queue: %d\n", response_queue);
    }
    Request request = generate_request(0, 0, NULL, 0.0f, "/CLIENT");
    
    Response buffer = malloc(sizeof(Message) + 1);
    
    printf("Sending request: %s\n", message_to_string(request));

    if (send_message(server, request) < 0) {
        perror("send_message");
    }

    printf("Waiting for response...\n");
    if (receive_message(response_queue, buffer) == -1) {
        perror("mq_receive failed");   
    }
    printf("Received message: %s\n", message_to_string(buffer));
}
