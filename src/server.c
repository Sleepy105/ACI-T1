#include "server.h"


int getServerCoil (Server* server, size_t n) {
    if (!server) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    if (n > SERVER_N_COILS) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    size_t holder_pos = n/8;
    uint8_t pos_in_holder = n%8;
    uint8_t pos_mask = 0x01 << pos_in_holder;

    return (server->coils[holder_pos] & pos_mask) ? 1 : 0;
}

int setServerCoil (Server* server, size_t n, bool value) {
    if (!server) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    if (n > SERVER_N_COILS) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    size_t holder_pos = n/8;
    uint8_t pos_in_holder = n%8;
    uint8_t pos_mask = 0x01 << pos_in_holder;
    
    if (value) {
        server->coils[holder_pos] |= pos_mask;
    }
    else {
        server->coils[holder_pos] &= ~pos_mask;
    }

    return SOCK_OK;
}

int serverInit (Server* server, char* self_address, uint16_t port, uint8_t backlog) {
    if (!server) {
        return -1;
    }

    server->socket = socketCreate();
    if (server->socket < 0) {
        return -1;
    }

    server->self_address = malloc((strlen(self_address)+1)*sizeof(char));
    strncpy(server->self_address, self_address, (strlen(self_address)+1));

    server->port = port;
    server->backlog = backlog;
    socketBind(server->socket, server->self_address, server->port);
    socketListen(server->socket, server->backlog);
    return 0;
}

void serverClose (Server* server) {
    if (!server) {
        return;
    }

    if (server->self_address) {
        free(server->self_address);
    }
    if (server->socket) {
        socketClose(server->socket);
    }
    server->socket = 0;
    server->port = 0;
    server->backlog = 0;
}

void serverProcess (Server* server, int client_socket) {
    
    char* str = calloc(SERVER_BUFFER_SIZE, sizeof(char));
    int len = 0;

	while (1) {
        len = recv(client_socket, str, SERVER_BUFFER_SIZE, 0);
        str[len] = 0;
        
        printf("Received: \"%s\", Lenght: %d\n", str, len);
        if (len == 1 && str[0] == '#') {
            break;
        }
        
        int p;
        for (p = 0; str[p] != '\0'; p++) {
            str[p] = toupper(str[p]);
        }
		
        socketWrite(client_socket, str, strlen(str));
    }
    free(str);
    socketClose(client_socket);
    printf("A Client has Disconnected\n");
}

void* thread_serverProcess (void* arg) {
    thread_args* args = (thread_args*)arg;
    int client_socket = args->socket;
    Server* server = args->server;
    serverProcess(server, client_socket);
    return 0;
}

void serverLoop (Server* server) {
    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t client_addr_length = sizeof(cli_addr);
        
        int new_socket = accept(server->socket, (struct sockaddr *) &cli_addr, &client_addr_length);
        if (new_socket == -1) {
            break;
        }
        
        printf("New client Connection\n");
        // serverProcess(new_socket);
        pthread_t thread;
        thread_args* args = calloc(1, sizeof(thread_args));
        args->socket = new_socket;
        args->server = server;
        pthread_create(&thread, NULL, &thread_serverProcess, args);
    }
}

void* thread_serverLoop (void* arg) {
    Server* server = arg;
    serverLoop(server);
    return 0;
}

/*
pthread_t threads[3];
int thread_IDs[3];
void* thread_retValues[3];
ThreadArgs thread_args[3];


// Create the threads
thread_IDs[THREAD_READINPUT] = pthread_create(&threads[THREAD_READINPUT], NULL, &serverProcess, client_socket);
*/
