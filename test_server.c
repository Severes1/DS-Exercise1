#include <mqueue.h>
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>

#define CLIENT_QUEUE "/client"

int main() {

    mq_unlink(CLIENT_QUEUE);

    struct mq_attr mqueue_attr;
    mqueue_attr.mq_maxmsg = MAX_MSG;
    mqueue_attr.mq_msgsize = sizeof(Message);

    mqd_t server = mq_open("/SERVER", O_WRONLY, 0666, &mqueue_attr); 
    mqd_t response_queue = mq_open(CLIENT_QUEUE, O_RDONLY | O_CREAT, 0666, &mqueue_attr);

    if (response_queue < 0) {
        perror("response_queue");
    }
    Request request = generate_request(0, 0, NULL, 0.0f, CLIENT_QUEUE);
    
    Response buffer = malloc(sizeof(Message) + 1);
    
    printf("Sending request: %s\n", message_to_string(request));

    if (mq_send(server, request, MSG_SIZE, 0) < 0) {
        perror("mq_send");        
    }

    if (mq_receive(response_queue, buffer, MSG_SIZE, NULL) == -1) {
        perror("mq_receive failed");   
    }
}
