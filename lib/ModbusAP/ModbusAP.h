#ifndef __MODBUSAP_H__
#define __MODBUSAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/** ERROR TYPES **/
#define INVALID_ADDRESS -1
#define TOO_MANY_VALUES -2

/**
 * @brief Send n_r registers to a host located in server_add
 * 
 * @param server_add Server address
 * @param port Communications Port
 * @param st_r Start Register
 * @param n_r Number of Registers to write
 * @param val Value of those registers
 * @return int8_t Return code. If OK: number of written registers. If ERROR: <0
 */
int8_t Write_multiple_regs(char* server_add, int port, uint32_t st_r, uint8_t n_r, int16_t* val);

/**
 * @brief Send n_c coils to a host located in server_add
 * 
 * @param server_add Server address
 * @param port Communications Port
 * @param st_c Start Coil
 * @param n_c Number of Coils to write
 * @param val Value of those coils
 * @return int16_t Return code. If OK: number of written coils. If ERROR: <0
 */
int16_t Write_multiple_coils(char* server_add, int port, int st_c, int n_c, bool* val);

/**
 * @brief Receive n_r registers from a host located in server_add
 * 
 * @param server_add Server address
 * @param port Communications Port
 * @param st_r Start Register
 * @param n_r Number of Registers to read
 * @param val Value of those registers
 * @return int8_t Return code. If OK: number of read registers. If ERROR: <0
 */
int8_t Read_h_regs(char* server_add, int port, int st_r, int n_r, int16_t* val);

/**
 * @brief Read n_c coils from a host located in server_add
 * 
 * @param server_add Server address
 * @param port Communications Port
 * @param st_c Start Coil
 * @param n_c Number of Coils to read
 * @param val Value of those coils
 * @return int16_t Return code. If OK: number of read coils. If ERROR: <0
 */
int16_t Read_coils(char* server_add, int port, int st_c, int n_c, bool* val);

int Get_request();
int Send_Response();

#endif