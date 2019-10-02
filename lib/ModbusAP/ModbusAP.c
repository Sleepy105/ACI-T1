#include "ModbusAP.h"

int8_t Write_multiple_regs(char* server_add, int port, uint32_t st_r, uint8_t n_r, int16_t* val) {
    struct sockaddr_in server_address = {0};
    if (!inet_aton(server_add, &server_address.sin_addr)) {
        return INVALID_ADDRESS;
    }

    if (n_r > 123) {
        return TOO_MANY_VALUES;
    }

    return 0;
}