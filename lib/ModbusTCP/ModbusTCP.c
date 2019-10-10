#include "ModbusTCP.h"


int Send_Modbus_request(char* server_add, uint16_t port, uint8_t* apdu, size_t apdu_size, uint8_t* r_apdu) {
    if (!r_apdu) {
        return -1;
    }
    // Generate the TI (transaction identifier)
    static uint16_t TI = 0;
    TI++;

    // assemble the PDU = APDU(SDU) + MBAP
    uint16_t PI = 0x0000;
    uint16_t length = apdu_size + 1;
    uint8_t unit_id = 0x01;
    uint16_t total_frame_size = 7+apdu_size;
    uint8_t* request_frame = (uint8_t*)malloc(total_frame_size*sizeof(uint8_t));
    if (!request_frame) {
        return -1;
    }

    request_frame[0] = (uint8_t)(TI>>8);
    request_frame[1] = (uint8_t)(TI);
    request_frame[2] = (uint8_t)(PI>>8);
    request_frame[3] = (uint8_t)(PI);
    request_frame[4] = (uint8_t)(length>>8);
    request_frame[5] = (uint8_t)(length);
    request_frame[6] = (uint8_t)(unit_id);
    for (int i = 7; i < total_frame_size; i++) {
        request_frame[i] = apdu[i-7];
    }

    // open a client socket and connect to the server
    int socket = socketCreate();
    if (socket < 0) {
        free(request_frame);
        return -1;
    }
    if (socketConnect(socket, server_add, port) < 0) {
        free(request_frame);
        return -1;
    }

    // send data
    if (socketWrite(socket, request_frame, length) < 0) {
        free(request_frame);
        return -1;
    }
    free(request_frame);
    // TODO timeout error

    // read response or timeout
    uint8_t r_mbap[7] = {0};
    if (socketRead(socket, r_mbap, 7) == 7) {
        return -1;
    }

    // if response, remove MBAP and PDU_R -> APDU_R
    int r_apdu_size = ((((uint16_t)r_mbap[4]) << 8) | r_mbap[5]) -1;
    
    r_apdu = (uint8_t*)malloc(r_apdu_size);
    if (!r_apdu) {
        return -1;
    }
    if (socketRead(socket, r_apdu, r_apdu_size) < r_apdu_size) {
        return -1;
    }

    // close the client socket
    socketClose(socket);

    // return APDU_R and 0 – ok, <0 – error (timeout)
    return 0;
}

int Receive_Modbus_response(int fd, uint8_t* APDU , size_t APDUlen) {
    struct sockaddr_in cli_addr;
    socklen_t client_addr_length = sizeof(cli_addr);
    
    // waits for TCP connection, saves (global) data socket id
    int fd2 = accept(fd, (struct sockaddr *) &cli_addr, &client_addr_length);
    if (fd2 < -1) {
        return -1;
    }

    // read MBAP of request PDU
    uint8_t r_mbap[7] = {0};
    if (socketRead(fd2, r_mbap, 7) == 7) {
        return -1;
    }

    // remove MBAP: TI and length (APDUlen + 1)
    uint16_t response_length = ((((uint16_t)r_mbap[4]) << 8) | r_mbap[5]) -1;
    if (APDUlen < response_length) {
        return -1;
    }
    // read response APDU
    if (socketRead(fd2, APDU, APDUlen) < 0) {
        return -1;
    }

    // returns: APDU and TI – ok, <0 – erro
    uint16_t TI = (((uint16_t)r_mbap[0]) << 8) | r_mbap[1];
    // TODO: Add TI and fd2 to a dict
    return TI;
}

int Send_Modbus_response(uint16_t TI, uint8_t* resp_apdu , size_t resp_apdu_size) {
    // TODO: Find fd2 from TI
    int fd2 = 0;
    
    // assembles PDU = APDU_R + MBAP (with TI)
    uint16_t PI = 0x0000;
    uint16_t length = resp_apdu_size + 1;
    uint8_t unit_id = 0x01;
    size_t pdu_size = resp_apdu_size+7;
    uint8_t* PDU = (uint8_t*)malloc(pdu_size*sizeof(uint8_t));

    PDU[0] = (uint8_t)(TI>>8);
    PDU[1] = (uint8_t)(TI);
    PDU[2] = (uint8_t)(PI>>8);
    PDU[3] = (uint8_t)(PI);
    PDU[4] = (uint8_t)(length>>8);
    PDU[5] = (uint8_t)(length);
    PDU[6] = (uint8_t)(unit_id);
    for (size_t i = 7; i < pdu_size; i++) {
        PDU[i] = resp_apdu[i-7];
    }
    
    //sends ModbusTCP response PDU
    size_t written_bytes = socketWrite(fd2, PDU, pdu_size);
    if (written_bytes < pdu_size) {
        free(PDU);
        return -1;
    }
    free(PDU);

    // returns: >0 – ok, <0 – erro
    return written_bytes;
}
