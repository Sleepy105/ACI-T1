#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
        printf("Server 0 started\n");
    }
    pthread_t thread;
    pthread_create(&thread, NULL, &thread_serverLoop, &server);

    // INIT CLIENTS
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        size_t num_of_chars = i ? (ceil(log10(i))+1) : 1;
        char* str = malloc((num_of_chars+1)*sizeof(char));
        snprintf(str, num_of_chars+1, "%d", i);
        if (clientInit(&client[i], "1", SERVER_ADDRESS_FOR_CLIENT_CONNECTION, SERVER_PORT_FOR_CLIENT_CONNECTION)) {
            printf("ERROR creating client %s\n", str);
        }
        else {
            printf("Client %s started\n", str);
        }
        free(str);
    }


    // RUN TESTS
    while (1) {
        for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
            getchar();
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
