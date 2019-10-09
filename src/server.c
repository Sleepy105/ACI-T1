#include "server.h"


int getServerCoil (Server* server, size_t n) {
    if (!server) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    if (n < 0 || n > N_COILS) {
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

    if (n < 0 || n > N_COILS) {
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


int serverInit (Server* server, uint16_t port) {
    server->socket = socketCreate();
    if (server->socket < 0) {
        return -1;
    }
    server->port = port;
    int res = socketBind(server->socket, SERVER_ADDRESS, server->port);
    socketListen(server->socket, 10);
    return 0;
}