#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#include <socket.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_BUFFER_SIZE 100
#define SERVER_N_REGS 20
#define SERVER_N_COILS 20

typedef struct {
    int socket;
    uint16_t port;
    uint8_t backlog;
    uint16_t h_regs[SERVER_N_REGS];
    uint8_t coils[SERVER_N_COILS];
} Server;

int getServerCoil (Server* server, size_t n);

int setServerCoil (Server* server, size_t n, bool value);

int serverInit (Server* server, uint16_t port, uint8_t backlog);

void serverProcess (int client_socket);

void serverLoop (Server* server);

void* thread_serverLoop (void* arg);

#endif