#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#include <socket.h>

#define CLIENT_BUFFER_SIZE 100
#define CLIENT_N_REGS 20
#define CLIENT_N_COILS 20

typedef struct {
    int socket;
    char* name;
    uint16_t port;
    uint16_t h_regs[CLIENT_N_REGS];
    uint8_t coils[CLIENT_N_COILS];
} Client;

int getClientCoil (Client* client, size_t n);

int setClientCoil (Client* client, size_t n, bool value);

int clientInit (Client* client, char* name, char* address, uint16_t port);

void clientProcess (Client* client);

void clientClose (Client* client);

#endif