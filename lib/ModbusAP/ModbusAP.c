//ModbusAP.c

#include "ModbusAP.h"

//SERVER
int server_init(char* server_ip, int port) {
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
     if(!(server_socket>=0)) printf("%s\n", "Error creating socket");
   
    
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(server_ip, &serv_addr.sin_addr);
    
    int res = bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(!(res >= 0))  printf("%s\n", "Error binding to socket");
    
    listen(server_socket, 5);
    return server_socket;
}

uint16_t Get_request (int fd , uint16_t op, uint16_t st , uint16_t n, uint16_t* val){
    uint16_t TI = Receive_Modbus_request (fd , APDU , APDUlen);
    // extract parameters from APDU ,
    // returns: TI and parameters ok, <0 error
    return 0;
}

uint16_t Send_response(uint16_t TI, uint16_t op, uint16_t st , uint16_t n, uint16_t* val){
    // prepare and send response APDU
    Send_Modbus_response (TI, APDU_R , APDU_Rlen);
    // returns: >0 ok, <0 error
    return 0;
}

/*----------------------------------------------------------------------------------------------------------*/

// CLIENT
int client_connect(char* server_ip, int port) {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(!(sock>=0)) printf("%s\n", "Error creating socket");
   
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(server_ip, &serv_addr.sin_addr);
    
    int res = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(!(res >= 0))  printf("%s\n", "Error connecting to socket");
    
    return sock;
}

uint8_t Write_multiple_regs(char* server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t* val) {
    
    //Verify the consistency
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
    apdu[1]=((st_r)>>8)) & 0xFF; apdu[2]=((st_r) & 0xFF);  
    apdu[3]=((n_r)>>8)) & 0xFF; apdu[4]=((n_r) & 0xFF);    
    apdu[5]=2*n_r;
    
    //Load values
    for(uint16_t i=0; i<n_r; i++){
        apdu[6+i*2]=((val[i])>>8) & 0xFF;
        apdu[6+i*2+1]=((val[i]) & 0xFF);   
    }
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, r_apdu)){
        return ERROR_REQUEST;
    }
    
    //Check the response
    uint8_t r_code = r_apdu[0];
    uint8_t r_val;
    if(r_code==0x10){
        r_val=NO_ERROR;
    }
    else {
        r_val=r_apdu[1]; // exception_code    
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return r_val;
}

uint8_t Read_h_regs(char* server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t* val){
    
    //Verify the consistency
    struct sockaddr_in server_address = {0};
    if (!inet_aton(server_add, &server_address.sin_addr)) {
        return INVALID_ADDRESS;
    }

    if (n_r > 125) {
        return TOO_MANY_VALUES;
    }

    //Malloc
    uint16_t apdu_size=5;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x03;
    apdu[1]=((st_r)>>8)) & 0xFF; apdu[2]=((st_r) & 0xFF);  
    apdu[3]=((n_r)>>8)) & 0xFF; apdu[4]=((n_r) & 0xFF);      
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, r_apdu)){
        return ERROR_REQUEST;
    }
    
    //Check the response
    uint8_t r_code=r_apdu[0];
    uint8_t r_val;
    if(r_code==0x03){
         r_val=NO_ERROR;
         uint16_t *val_count r_apdu[1]/2;
         *val malloc(*val_count);

        for (uint16_t i=0; i < n_r; i++) {
            (*val)[i] = ((r_apdu[2+i*2]) << 8) | (r_apdu[2+i*2+1]);
        } 
    }
    else {
        r_val==r_apdu[1]; // exception_code 
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return 0;
}

void client_close(int sock) {
    close(sock);
}

/*
uint16_t Write_multiple_coils(char* server_add, int port, int st_c, int n_c, bool* val){
 //Malloc
    uint16_t apdu_size=6+n_r/8;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x0F;
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
    if(r_code==0x0F){
        //no error 
    }
    else {
        
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return 0;
}

uint16_t Read_coils(char* server_add, int port, int st_c, int n_c, bool* val){
 //Malloc
    uint16_t apdu_size=5;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x01;
    apdu[1]=((st_r) & 0xFF00)>>8; apdu[2]=((st_r) & 0x00FF)>>0;  
    apdu[3]=((n_r) & 0xFF00)>>8; apdu[4]=((n_r) & 0x00FF)>>0;    
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, r_apdu)){
        //error
    }
    
    uint8_t r_code = r_apdu[0];
    uint8_t r_val;
    if(r_code==0x01){
        //no error 
    }
    else {
        
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return 0;
}
*/