#include "server.h"

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