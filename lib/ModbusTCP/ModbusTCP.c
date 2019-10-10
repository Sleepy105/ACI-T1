//  ModbusTCP.c

#include "ModbusTCP.h"

// CLIENT
uint8_t Send_Modbus_request(char* server_add, uint16_t port, uint8_t* APDU, uint16_t APDUlen, uint8_t** APDU_R) {
    // Header information
    uint16_t transaction_id = 0;
    uint16_t protocol_id = 0;
    uint16_t length = 1 + APDUlen;
    uint8_t unit_id = 0; // Ignored
    uint16_t APDU_Rlen;

    // Alloc buffer
    uint16_t tcppdu_size = 7 + APDUlen;
    uint8_t* tcppdu = malloc(tcppdu_size);
    
    // Write tcppdu header
    tcppdu[0] = (transaction_id >> 8) & 0xFF;
    tcppdu[1] = transaction_id & 0XFF;
    tcppdu[2] = (protocol_id >> 8) & 0xFF;
    tcppdu[3] = protocol_id & 0XFF;
    tcppdu[4] = (length >> 8) & 0xFF;
    tcppdu[5] = length & 0XFF;	
    tcppdu[6] = unit_id;
    
    // Write tcppdu payload
    memcpy(tcppdu + 7, APDU, APDUlen);
    
    // Open client socket and connects to server
    int s = client_connect(server_add, port);

    // Send
    if(!socketWrite(s, tcppdu, tcppdu_size)) return -1;
    
    // Free mem
    free(tcppdu);
    
    // Receive
    if(!(socketRead(s, APDU_R, APDU_Rlen))) return -1;


    // A PARTIR DE AQUI DEVE ESTAR MAL, NAO SEI COMO SE FAZ AO CERTO
    // if response, remove MBAP , PDU_R -> APDU_R
    // Receive mbap
    uint8_t mbap[7];

    if (!socketRead(s, mbap, 7)) {
        return -1;
    }

    transaction_id = (mbap[0] << 8) |  mbap[1]; 
    APDUlen = (mbap[4] << 8) |  mbap[5]; 
    (APDUlen)--;
    
    // Receive apdu
    APDU = malloc(APDUlen);
    if(!(socketRead(s, APDU, APDUlen))) return -1;

    // closes TCP client socket with server
    client_close(s);

    // returns: APDU_R and 0 ok, <0 error (timeout)
    return 0;
}

// SERVER
uint16_t Receive_Modbus_request(int s, uint8_t* APDU, uint16_t* APDUlen) {

    // waits for TCP connection, saves (global) data socket id
    struct sockaddr_in cli_addr;
    socklen_t client_addr_length = sizeof(cli_addr);
    int s2=accept(s, (struct sockaddr *) &cli_addr, &client_addr_length); 
    if (s2 == -1) return -1;  

    // Receive mbap
    uint8_t mbap[7];
    if (!socketRead(s, mbap, 7)) {
        return -1;
    }
    
    // Process mbap
    uint16_t transaction_id = (mbap[0] << 8) |  mbap[1]; 
    *APDUlen = (mbap[4] << 8) |  mbap[5]; 
    (*APDUlen)--;
    
    // Receive apdu
    APDU = malloc(*APDUlen);
    if(!(socketRead(s2, APDU, *APDUlen))) return -1;

    //return TI
    return transaction_id;
}

int Send_Modbus_response(uint16_t TI, uint8_t* APDU_R, uint16_t APDU_Rlen) {
    // Header information
    uint16_t protocol_id = 0;
    uint16_t length = 1 + APDU_Rlen;
    uint8_t unit_id = 0; // Ignored
    
    // Alloc buffer
    uint16_t tcppdu_size = 7 + APDU_Rlen;
    uint8_t* tcppdu = malloc(tcppdu_size);
    
    // Write tcppdu header
    tcppdu[0] = (TI >> 8) & 0xFF;
    tcppdu[1] = TI & 0XFF;
    tcppdu[2] = (protocol_id >> 8) & 0xFF;
    tcppdu[3] = protocol_id & 0XFF;
    tcppdu[4] = (length >> 8) & 0xFF;
    tcppdu[5] = length & 0XFF;	
    tcppdu[6] = unit_id;
    
    // Write tcppdu payload
    memcpy(tcppdu + 7, APDU_R, APDU_Rlen);

    // waits for TCP connection, saves (global) data socket id
    struct sockaddr_in cli_addr;
    socklen_t client_addr_length = sizeof(cli_addr);
    // como faço accept sem um socket????
    int s2; //=accept(s, (struct sockaddr *) &cli_addr, &client_addr_length); 
    //if (s2 == -1) return -1;  

    // Send
    if(!(socketWrite(s2, tcppdu, tcppdu_size))) return -1;
    
    // Free mem
    free(tcppdu);

    return 0;
}