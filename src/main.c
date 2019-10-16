#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socket.h"
#include "server.h"
#include "client.h"

#define BUFFER_LENGTH 32

#define SERVER_ADDRESS_FOR_CLIENT_CONNECTION "192.168.113.33"
#define SERVER_PORT_FOR_CLIENT_CONNECTION 5502
#define NUMBER_OF_CLIENTS 2

Server server;
Client client[NUMBER_OF_CLIENTS];


// int main(int argc, char const *argv[]) {
int main() {
    // INIT SERVER
    if (serverInit(&server, SERVER_ADDRESS_FOR_CLIENT_CONNECTION, SERVER_PORT_FOR_CLIENT_CONNECTION, 10)) {
        printf("Error starting the server\n");
        return -1;
    }
    else {
        printf("Server 1 started\n");
    }
    pthread_t thread;
    pthread_create(&thread, NULL, &thread_serverLoop, &server);

    // INIT CLIENTS
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        if (clientInit(&client[i], "1", SERVER_ADDRESS_FOR_CLIENT_CONNECTION, SERVER_PORT_FOR_CLIENT_CONNECTION)) {
            printf("ERROR creating client 1\n");
        }
        else {
            printf("Client 1 started\n");
        }
    }


    // RUN TESTS
    while (1) {
        for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
            clientReadRegs(&client[i], 0, 2);
        }
    }


    // Cleanup
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        clientClose(&client[i]);
    }
    serverClose(&server);
    printf("Connection Closed\n");

    return 0;
}
