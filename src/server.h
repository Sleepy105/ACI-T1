#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdbool.h>
#include <socket.h>

#define SERVER_ADDRESS "127.0.0.1"
#define N_REGS 20
#define N_COILS 20

typedef struct {
    int socket;
    uint16_t port;
    uint16_t h_regs[N_REGS];
    uint8_t coils[N_COILS];
    char* buffer_out;
    char* buffer_in;
} Server;

int getServerCoil (Server* server, size_t n);

int setServerCoil (Server* server, size_t n, bool value);

int serverInit (Server* server, uint16_t port);

#endif