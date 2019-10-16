#include "client.h"

int getClientCoil (Client* client, size_t n) {
    if (!client) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    if (n > CLIENT_N_COILS) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    size_t holder_pos = n/8;
    uint8_t pos_in_holder = n%8;
    uint8_t pos_mask = 0x01 << pos_in_holder;

    return (client->coils[holder_pos] & pos_mask) ? 1 : 0;
}

int setClientCoil (Client* client, size_t n, bool value) {
    if (!client) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    if (n > CLIENT_N_COILS) {
        return SOCK_VALUE_OUT_OF_BOUNDS;
    }

    size_t holder_pos = n/8;
    uint8_t pos_in_holder = n%8;
    uint8_t pos_mask = 0x01 << pos_in_holder;
    
    if (value) {
        client->coils[holder_pos] |= pos_mask;
    }
    else {
        client->coils[holder_pos] &= ~pos_mask;
    }

    return SOCK_OK;
}

int clientInit (Client* client, char* name, char* address, uint16_t port) {
    if (!client) {
        return -1;
    }

    client->socket = socketCreate();
    if (client->socket < 0) {
        return -1;
    }

    client->name = malloc((strlen(name)+1)*sizeof(char));
    strncpy(client->name, name, (strlen(name)+1));

    int res = socketConnect(client->socket, address, port);
    if (res < 0) {
        return -1;
    }

    return 0;
}

void clientProcess (Client* client) {
    if (!client) {
        return;
    }

    char* str = calloc(CLIENT_BUFFER_SIZE, sizeof(char));

    printf("\n---> [client %s] Enter string to send: ", client->name);
    scanf("%s", str);
    socketWrite(client->socket, str, strlen(str));
    
    int len = recv(client->socket, str, CLIENT_BUFFER_SIZE, 0);
    if (len <= 0) {
        printf("[client %s] ERROR receiving from server\n", client->name);
    }
    
    printf("[client %s] Received from server: %s", client->name, str);
    return;
}

void clientClose (Client* client) {
    socketClose(client->socket);
    client->socket = 0;
    if (client->name) {
        free(client->name);
    }
}
