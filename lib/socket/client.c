// client.c

#include "ModbusAP.h"

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_ADD "127.0.0.1"
#define PORT 5502
#define TESTE 0

void c_write_multiple_regs(char* server_add, int port) {
    int size;
    int pos;
    
    printf("\n---> Enter size to write multiple regs: ");
    scanf("%d", size);

    printf("\n---> Enter pos to write multiple regs: ");
    scanf("%d", pos);    

    printf("\nW %d regs at position %d: ", size, pos);
    
    uint16_t* values = malloc(size);
    for (int i = 0; i < size; i++) {
        printf("\n---> Enter values[%d]: ", i);
        scanf("%d", values[i]);
    }
    
    uint8_t result = Write_multiple_regs(server_add, port, pos, size, values); 
    
    if (result) {
        printf("error code: %d\n", result);
    }
    else {
        printf("successful:");
        for (int i = 0; i < size; i++) {
            printf(" %hu", values[i]);
        }
        printf("\n");
    }
    
    free(values);
}

void c_read_h_regs(char* server_add, int port) {
    int size;
    int pos;
    
    printf("\n---> Enter size to read h regs: ");
    scanf("%d", size);

    printf("\n---> Enter pos to read r regs: ");
    scanf("%d", pos);    

    printf("\nR %d regs at position %d: ", size, pos);
    uint16_t* values;

    uint8_t result = Read_h_regs(server_add, port, pos, size, &values);
    
    if (result) {
        printf("error code: %d\n", result);
    }
    else {
        printf("successful:");
        for (int i = 0; i < size; i++) {
            printf(" %hu", values[i]);
        }
        printf("\n");
    }
    
    free(values);
}

int main(int argc, char* argv[]) {
    int s = client_connect(SERVER_ADD, PORT);
    printf("Connected to server\n");
    int teste=TESTE;

    while (1) {
        switch (teste) {
            case 0:
                c_write_multiple_regs(SERVER_ADD, PORT);
                break;
            case 1:
                c_read_h_regs(SERVER_ADD, PORT);
                break;
            default:
                break;
        }
        
    }
    
    client_close(s);
    printf("Disconnected from server\n");
    return 0;
}