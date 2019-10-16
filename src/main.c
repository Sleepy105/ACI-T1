#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socket.h"
#include "server.h"
#include "client.h"

#define BUFFER_LENGTH 32

Server server;

Client client1;
Client client2;

/* bool test_1 (void* server, void* client) {
    int res = client.write_multiple_regs(server);
    
}*/

// int main(int argc, char const *argv[]) {
int main() {
    if (serverInit(&server, 5502, 10)) {
        printf("Error starting the server\n");
        return -1;
    }
    else {
        printf("Server 1 started\n");
    }
    pthread_t thread;
    pthread_create(&thread, NULL, &thread_serverLoop, &server);

    if (clientInit(&client1, "1", "127.0.0.1", 5502)) {
        printf("ERROR creating client 1\n");
    }
    else {
        printf("Client 1 started\n");
    }
    if (clientInit(&client2, "2", "127.0.0.1", 5502)) {
        printf("ERROR creating client 2\n");
    }
    else {
        printf("Client 2 started\n");
    }

    while (1) {
        clientProcess(&client1);
        clientProcess(&client2);
    }

    clientClose(&client1);
    clientClose(&client2);
    printf("Connection Closed\n");

    return 0;
}
