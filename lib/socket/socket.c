#include "socket.h"

int socketCreate () {
    return socket(PF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in prepareSocketData (char* address, uint16_t port) {
    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_aton(address, &server_address.sin_addr);
    /*if (!inet_aton(address, &server_address.sin_addr)) {
        return NULL;
    }*/
    return server_address;
}


int socketConnect (int socket, char* address, uint16_t port) {
    struct sockaddr_in server_address = prepareSocketData(address, port);
    return connect(socket, (struct sockaddr*) &server_address, sizeof(server_address));
}


int socketBind (int socket, char* address, uint16_t port) {
    struct sockaddr_in server_address = prepareSocketData(address, port);
    return bind(socket, (struct sockaddr*) &server_address, sizeof(server_address));
}


int socketListen (int socket, int backlog) {
    return listen(socket, backlog);
}


bool socketClose (int socket) {
    close(socket);
    return false;
}


int socketWrite (int socket, void* buffer, size_t buffer_size) {
    return send(socket, buffer, buffer_size, 0);
}


int socketRead (int socket, void* buffer, size_t buffer_size) {
    return recv(socket, buffer, buffer_size, 0);
}
