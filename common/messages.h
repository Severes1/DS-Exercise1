#define QUEUE_NAME "/SERVER"
#define MSG_SIZE 300 // depends on MAX_STRING_SIZE and MAX_RETURN_QUEUE_NAME
#define MAX_MSG 10
#define MAX_STRING_SIZE 256
#define MAX_RETURN_QUEUE_NAME 32
#define RESPONSE_SUCCESS_FLAG 0

typedef struct message {
    char status;
    int key;
    char value1[MAX_STRING_SIZE];
    char return_queue[MAX_RETURN_QUEUE_NAME];
    float value2;
} Message, *Request, *Response;


Request generate_request(char status, int key, char * value1, float value2, char * return_queue);
Response generate_response(char status, int key, char * value1, float value2);

typedef struct Connection *Connection;
Connection create_connection_read(char * name);
Connection open_connection_write(char * name);
void close_connection(Connection connection);
void remove_connection(char * name);
int send_message(Connection connection, Message * message);
int receive_message(Connection connection, Message * buffer);
char * message_to_string(Message * message);
