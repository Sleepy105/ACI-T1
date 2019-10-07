//  device.c

#include "device.h"

device* device_new() {
    device* d = (device*)calloc(1, sizeof(device));
    d->size = DEVICE_SIZE;
    return d;
}

void device_free(device* d) {
    free(d);
}

uint16_t device_get_size(device* d) {
    return d->size;
}

void device_write_reg(device* d, uint16_t n, uint16_t val) {
    d->regs[n] = val;
}

uint16_t device_read_reg(device* d, uint16_t n, uint16_t val) {
    return d->regs[n];
}