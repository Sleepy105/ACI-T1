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

    client->name = malloc((strlen(name)+1)*sizeof(char));
    strncpy(client->name, name, (strlen(name)+1));

    client->server_address = malloc((strlen(address)+1)*sizeof(char));
    strncpy(client->server_address, address, (strlen(address)+1));

    client->port = port;

    return 0;
}


void clientClose (Client* client) {
    if (client->name) {
        free(client->name);
    }
    if (client->server_address) {
        free(client->server_address);
    }
    client->port = 0;
}

int clientReadRegs (Client* client, uint16_t start, uint16_t n) {
    if (!client) {
        return;
    }

    uint16_t res = Read_h_regs(client->server_address, client->port, start, n, client->h_regs);
    printf("[client %s] H_REGS:\n\t", client->name);
    for (uint16_t i = 0; i < CLIENT_N_REGS; i++) {
        printf("%d, ", client->h_regs[i]);
    }
    printf("\n");

    return res;
}

int clientWriteRegs (Client* client, uint16_t start, uint16_t n) {
    if (!client) {
        return;
    }

    uint16_t res = Write_multiple_regs(client->server_address, client->port, start, n, client->h_regs);
    printf("[client %s] H_REGS:\n\t", client->name);
    for (uint16_t i = 0; i < CLIENT_N_REGS; i++) {
        printf("%d, ", client->h_regs[i]);
    }
    printf("\n");

    return res;
}

int clientReadCoils (Client* client, uint16_t start, uint16_t n) {
    if (!client) {
        return;
    }

    uint16_t res = Read_coils(client->server_address, client->port, start, n, client->coils);
    printf("[client %s] COILS:\n\t", client->name);
    for (uint16_t i = 0; i < CLIENT_N_COILS; i++) {
        printf("%d, ", client->coils[i]);
    }
    printf("\n");

    return res;
}

int clientWriteCoils (Client* client, uint16_t start, uint16_t n) {
    if (!client) {
        return;
    }

    uint16_t res = Write_multiple_coils(client->server_address, client->port, start, n, client->coils);
    printf("[client %s] COILS:\n\t", client->name);
    for (uint16_t i = 0; i < CLIENT_N_COILS; i++) {
        printf("%d, ", client->coils[i]);
    }
    printf("\n");

    return res;
}
