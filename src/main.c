#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socket.h"
#include "server.h"

#define BUFFER_LENGTH 32

Server server;

/* bool test_1 (void* server, void* client) {
    int res = client.write_multiple_regs(server);
    
}*/

int main(int argc, char const *argv[])
{
    serverInit(&server, 5502);

    
    int socket = socketCreate();
    printf("Socket: %d\n", socket);
    int res = socketConnect(socket, "127.0.0.1", 5502);
    printf("Res: %d\n", res);

    char buf[BUFFER_LENGTH] = {0};
    while (1) {
        scanf("%s", buf);
        if (strnlen(buf, BUFFER_LENGTH) == 1 && buf[0] == '\\') {
            // Close the connection if a single '\' is sent
            break;
        }

        // Send the buffer contents
        int r = socketWrite(socket, buf, strnlen(buf, BUFFER_LENGTH));
        if (r < 0) {
            printf("Error sending: %d\n", r);
        }
        else {
            printf("SENT     %d bytes: %s\n", r, buf);
        }

        // Clear the buffer
        for (int i = 0; i < BUFFER_LENGTH; i++) {
            buf[i] = 0;
        }

        // Read the socket
        r = socketRead(socket, buf, BUFFER_LENGTH);
        if (r < 0) {
            printf("Error receiving: %d\n", r);
        }
        else {
            printf("RECEIVED %d bytes: %s\n", r, buf);
        }

        printf("%s\n", buf);
    }

    socketClose(socket);
    printf("Connection Closed\n");

    return 0;
}
