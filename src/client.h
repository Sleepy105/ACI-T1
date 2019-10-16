#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#include <socket.h>
#include "ModbusAP.h"

#define CLIENT_BUFFER_SIZE 100
#define CLIENT_N_REGS 20
#define CLIENT_N_COILS 20

typedef struct {
    char* name;
    char* server_address;
    uint16_t port;
    uint16_t h_regs[CLIENT_N_REGS];
    uint8_t coils[CLIENT_N_COILS];
} Client;

int getClientCoil (Client* client, size_t n);

int setClientCoil (Client* client, size_t n, bool value);

int clientInit (Client* client, char* name, char* address, uint16_t port);

void clientClose (Client* client);

int clientReadRegs (Client* client, uint16_t start, uint16_t n);

int clientWriteRegs (Client* client, uint16_t start, uint16_t n);

int clientReadCoils (Client* client, uint16_t start, uint16_t n);

int clientWriteCoils (Client* client, uint16_t start, uint16_t n);

#endif