//  device_.h
#ifndef device_h
#define device_h

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define DEVICE_SIZE 65535

typedef struct _device {
    uint16_t regs[DEVICE_SIZE];
    //bool_t coils[DEVICE_SIZE];
    uint16_t size;
} device;

device* device_new();
void device_free(device* d);
uint16_t device_get_size(device* d);
void device_write_reg(device* d, uint16_t n, uint16_t val);
uint16_t device_read_reg(device* d, uint16_t pos, uint16_t val);

#endif