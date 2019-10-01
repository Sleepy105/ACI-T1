/*03 (0x03) Read Holding Register
  16 (0x10) Write Multiple Registers
  01 (0x01) Read Coils
  15 (0x0F) Write Multiple Coils*/

/*.................ModbusAP.c................*/

#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "ModbusAP.h"


// Read_h_regs( ) / Write_multiple_regs ()
// Get_Request( ) / Send_Response ()
