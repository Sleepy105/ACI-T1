/*.................server.c................*/

#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5502

void socket_write(int s, char* str) {
    /*long pos = 0;
    while (pos < strlen(str) + 1) {
        pos += write(s, str + pos, strlen(str) + 1 - pos);
    }*/
	send(s, str, strlen(str), 0);
}


void socket_close(int s) {
    close(s);
}


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

void server_process(int client_socket) {
    
    int str_len = 255;
    char str[str_len];
	while (1) {
		for(int i = 0; i < str_len; i++){
			str[i] = 0;
		}
		
        int len = recv(client_socket, str, str_len, 0);
        
        printf("Received: \"%s\", Lenght: %d\n", str, len);
        if (len == 1 && str[0] == '#') { break; }
        
        int p;
        for (p = 0; str[p] != '\0'; p++) {
            str[p] = toupper(str[p]);
        }
		
        socket_write(client_socket, str);
    }
    
    socket_close(client_socket);
}

void server_loop(int server_socket) {
    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t client_addr_length = sizeof(cli_addr);
        
        int new_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &client_addr_length);
        if (new_socket == -1) { break; }
        
        printf("Connected to client\n");
        server_process(new_socket);
        printf("Disconnected from client\n");
    }
}

void server_close(int server_socket) {
    socket_close(server_socket);
}

int main() {
    int server_socket = server_init(SERVER_IP, PORT);
    printf("Server started\n");
    server_loop(server_socket);
    server_close(server_socket);
}
