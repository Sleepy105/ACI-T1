#include "ModbusAP.h"

int8_t Write_multiple_regs(char* server_add, int port, uint32_t st_r, uint8_t n_r, int16_t* val) {
    struct sockaddr_in server_address = {0};
    if (!inet_aton(server_add, &server_address.sin_addr)) {
        return INVALID_ADDRESS;
    }

    if (n_r > 123) {
        return TOO_MANY_VALUES;
    }
    
    //Malloc
    uint16_t apdu_size=6+n_r*2;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x10;
    apdu[1]=((st_r) & 0xFF00)>>8; apdu[2]=((st_r) & 0x00FF)>>0;  
    apdu[3]=((n_r) & 0xFF00)>>8; apdu[4]=((n_r) & 0x00FF)>>0;    
    apdu[5]=2*n_r;
    
    //Load values
    for(int i=0; i<n_r; i++){
        apdu[6+i]=((val[i]) & 0xFF00)>>8; 
        apdu[6+i+1]=((val[i]) & 0x00FF)>>0;   
    }
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, r_apdu)){
        //error
    }
    
    uint8_t r_code = r_apdu[0];
    uint8_t r_val;
    if(r_code==0x10){
        //no error 
    }
    else {
        
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return 0;
}

uint8_t Read_h_regs(char* server_add, int port, uint32_t st_r, uint8_t n_r, int16_t* val){
    //Malloc
    uint16_t apdu_size /*?????*/;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x03;
    apdu[1]=((st_r) & 0xFF00)>>8; apdu[2]=((st_r) & 0x00FF)>>0;  
    apdu[3]=((n_r) & 0xFF00)>>8; apdu[4]=((n_r) & 0x00FF)>>0;    
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, r_apdu)){
        //error
    }
    
    uint8_t r_code = r_apdu[0];
    uint8_t r_val;
    if(r_code==0x03){
        //no error 
    }
    else {
        
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return 0;
}

