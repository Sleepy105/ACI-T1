// server.c

#include "ModbusAp.h"
#include "device.h"

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5502

/*
int main(int argc, char* argv[]) {
    int s = server_init(SERVER_IP, PORT);

    printf("Connected to client\n");

    device* driver = device_new();

    close(s);
    device_free(driver);
    
    return 0;
}*/