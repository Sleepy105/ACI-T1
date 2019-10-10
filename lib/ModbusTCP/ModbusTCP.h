#ifndef __MODBUSTCP_H__
#define __MODBUSTCP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "socket.h"
#include "ModbusAP.h"

int Send_Modbus_response(uint16_t TI, uint8_t* APDU_R, uint16_t APDU_Rlen);
uint8_t Send_Modbus_request(char* server_add, uint16_t port, uint8_t* APDU, uint16_t APDUlen, uint8_t** APDU_R);
uint16_t Receive_Modbus_request(int s, uint8_t* APDU, uint16_t* APDUlen);

#endif