#include <mqueue.h>
#include "messages.h"

int main() {
    Request message;
    message = generate_request(0, 0, "Haha", 0.0, "/SERVER");
    Connection server = open_connection_write("/SERVER");
    send_message(server, message);
}
