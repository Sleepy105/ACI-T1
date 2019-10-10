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

    uint16_t transaction_id;
    uint8_t* req_apdu=0;
    uint16_t req_apdu_size;

    // Receive apdu
    transaction_id = Receive_Modbus_request (fd , req_apdu , &req_apdu_size);

    // extract parameters from APDU
    op = req_apdu[0];
    switch (op) {
        case READ_REGS:
            st = ((req_apdu[1]) << 8) | (req_apdu[2]);
            n = ((req_apdu[3]) << 8) | (req_apdu[4]);
    
            if (n < 1 || n > 125) {
                server_create_err_apdu(READ_REGS, ILLEGAL_DATA_VALUE, &req_apdu, &req_apdu_size);
            }
            /*acho que nao é preciso pq é request e na 
            op 0x03 nao tem dados-introd._to_modbus-slide7*/
            /*else{
                for (uint16_t i = 0; i < n; i++) {
                    val[i]= (req_apdu[st+i]>>8) & 0xFF;
                    val[i+1]=req_apdu[st+i+1] & 0xFF;
                    //val[st + i] = ((req_apdu[2+i*2]) << 8) | (req_apdu[2+i*2+1]);  
                }
            }*/
            break;
            
        case WRITE_REGS:
            st = ((req_apdu[1]) << 8) | (req_apdu[2]);
            n = ((req_apdu[3]) << 8) | (req_apdu[4]);
            uint8_t byte_count = req_apdu[5];
    
            if (n < 1 || n > 123 || byte_count != n * 2) {
                server_create_err_apdu(WRITE_REGS, ILLEGAL_DATA_VALUE, &req_apdu, &req_apdu_size);
            }

            else{
                for (uint16_t i = 0; i < n; i++) {   
                    val[st+i] = (req_apdu[6+i*2]>>8) & 0xFF;
                    val[st+i+1] = req_apdu[6+i*2+1] & 0xFF;
                }
            }    
            break;

        default:
            server_create_err_apdu(op, ILLEGAL_FUNCTION, &req_apdu, &req_apdu_size);
            break;
    }

    // Free mem
    free(req_apdu);      
    
    // returns: TI and parameters ok, <0 error
    return transaction_id;
}    


uint16_t Send_response(uint16_t TI, uint16_t op, uint16_t st , uint16_t n, uint16_t* val){
    // prepare and send response APDU
    uint8_t* resp_apdu;
    uint16_t resp_apdu_size;

    // Process apdu
    switch (op) {
        case READ_REGS:
            if (n < 1 || n > 125) {
                server_create_err_apdu(READ_REGS, ILLEGAL_DATA_VALUE, &resp_apdu, &resp_apdu_size);
            }

            else {
                resp_apdu_size = 2 + n * 2;
                resp_apdu = malloc(resp_apdu_size); 
                if(!(resp_apdu)) return -1;
        
                resp_apdu[0] = READ_REGS;
                resp_apdu[1] = n * 2;
        
                for (uint16_t i = 0; i < n; i++) {
                    resp_apdu[2 + i * 2] = (val[st+i] >> 8) & 0xFF; 
                    resp_apdu[2 + i * 2 + 1] = val[st+i] & 0xFF;  
                }
            }break;
            
        case WRITE_REGS:
    
            if (n < 1 || n > 123) {
                server_create_err_apdu(WRITE_REGS, ILLEGAL_DATA_VALUE, &resp_apdu, &resp_apdu_size);
            }
            else{
            resp_apdu_size = 5;
            resp_apdu = malloc(resp_apdu_size);           
            if(!(resp_apdu)) return -1;

            resp_apdu[0] = WRITE_REGS;
            resp_apdu[1]=((st)>>8) & 0xFF; resp_apdu[2]=((st) & 0xFF);  
            resp_apdu[3]=((n)>>8) & 0xFF; resp_apdu[4]=((n) & 0xFF);          
            //memcpy(resp_apdu, val, 5);
            }
        break;

        default:
            server_create_err_apdu(op, ILLEGAL_FUNCTION, &resp_apdu, &resp_apdu_size);
        break;
    }

    // Send response
    Send_Modbus_response (TI, resp_apdu , resp_apdu_size);  

    // Free mem
    free(resp_apdu);      

    // returns: >0 ok, <0 error
    return 0;
}

void server_create_err_apdu(uint8_t function_code, uint8_t exception_code, uint8_t** resp_apdu, uint16_t* resp_apdu_size) {
    *resp_apdu_size = 2;
    *resp_apdu = malloc(*resp_apdu_size);
    (*resp_apdu)[0] = 0x80 + function_code;
    (*resp_apdu)[1] = exception_code;
}

void server_close(int sock) {
    close(sock);
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

uint16_t Write_multiple_regs(char* server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t* val) {
    
    //Verify the consistency
    struct sockaddr_in server_address = {0};
    if (!inet_aton(server_add, &server_address.sin_addr)) {
        return INVALID_ADDRESS;
    }
    
    //Malloc
    uint16_t apdu_size=6+n_r*2;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x10;
    apdu[1]=((st_r)>>8) & 0xFF; apdu[2]=((st_r) & 0xFF);  
    apdu[3]=((n_r)>>8) & 0xFF; apdu[4]=((n_r) & 0xFF);    
    apdu[5]=2*n_r;
    
    //Load values
    for(uint16_t i=0; i<n_r; i++){
        apdu[6+i*2]=((val[i])>>8) & 0xFF;
        apdu[6+i*2+1]=((val[i]) & 0xFF);   
    }
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, &r_apdu)){
        return ERROR_REQUEST;
    }
    
    //Check the response
    uint8_t r_code = r_apdu[0];
    uint16_t r_val;
    if(r_code==WRITE_REGS){
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

uint16_t Read_h_regs(char* server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t* val){
    
    //Verify the consistency
    struct sockaddr_in server_address = {0};
    if (!inet_aton(server_add, &server_address.sin_addr)) {
        return INVALID_ADDRESS;
    }

    //Malloc
    uint16_t apdu_size=5;
    uint8_t* apdu=malloc(apdu_size);
    
    //Write header
    apdu[0]=0x03;
    apdu[1]=((st_r)>>8) & 0xFF; apdu[2]=((st_r) & 0xFF);  
    apdu[3]=((n_r)>>8) & 0xFF; apdu[4]=((n_r) & 0xFF);      
    
    //Send & Receive
    uint8_t* r_apdu;
    if(!Send_Modbus_request (server_add,  port, apdu, apdu_size, &r_apdu)){
        return ERROR_REQUEST;
    }
    
    //Check the response
    uint8_t r_code=r_apdu[0];
    uint16_t r_val = 0;
    
    if(r_code==READ_REGS){
        r_val = r_apdu[1]/2;

        for (uint16_t i=0; i < n_r; i++) {
            val[st_r + i] = ((r_apdu[2+i*2]) << 8) | (r_apdu[2+i*2+1]);
        } 
    }
    else {
        r_val = r_apdu[1]; // exception_code 
    }
  
    //Free
    free(apdu);
    free(r_apdu);
        
    return r_val;
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