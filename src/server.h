#ifndef __SERVER_H__
#define __SERVER_H__

#include <socket.h>

#define SERVER_ADDRESS "127.0.0.1"

typedef struct {
    int socket;
    uint16_t port;
    char* buffer_out;
    char* buffer_in;
} Server;

int serverInit (Server* server, uint16_t port);

#endif