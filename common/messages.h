#define QUEUE_NAME "/SERVER"
#define MSG_SIZE 300 // depends on MAX_STRING_SIZE and MAX_RETURN_QUEUE_NAME
#define MAX_MSG 10
#define MAX_STRING_SIZE 256
#define MAX_RETURN_QUEUE_NAME 32
#define RESPONSE_SUCCESS_FLAG 0

/* Definition of structure for both requests and responses
 * There is no need to ntohl and htonl because the communication
 * is contained within one machine. */
typedef struct message {
    char status;
    int key;
    char value1[MAX_STRING_SIZE];
    char return_queue[MAX_RETURN_QUEUE_NAME];
    float value2;
} Message, *Request, *Response;

/* Generate a pointer to a Message structure with the given data */
Request generate_request(char status, int key, char * value1, float value2, char * return_queue);

/* Generate a pointer to a Message structure without the return_queue */
Response generate_response(char status, int key, char * value1, float value2);

/* Abstract datatype encapsulating the mq_id and name of the queue */
typedef struct Connection *Connection;

/* Open a queue read_only */
Connection create_connection_read(char * name);

/* Open a queue for writing only */
Connection open_connection_write(char * name);

/* Close the queue */
void close_connection(Connection connection);

/* Unlink the queue */
void remove_connection(char * name);

/* Send the given message over the given message queue */
int send_message(Connection connection, Message * message);

/* Block until a message is received on the given queue */
int receive_message(Connection connection, Message * buffer);

/* Generate a printable string representing the message */
char * message_to_string(Message * message);
