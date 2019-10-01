
/*...............client.c...............*/

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5502


void socket_write(int s, char* str) {

	send(s, str, strlen(str), 0);
}

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

void client_process(int sock) {
    int str_len = 255;
    char str[str_len];
	while (1) {
		for (int i = 0; i < str_len; i++){
			str[i] = '\0';
		}
        printf("\n---> Enter string to send: ");
        scanf("%s", str);
        socket_write(sock, str);
		
        int len = recv(sock, str, str_len, 0);
        if (len == 0) { break; }
        
        printf("Received from server: %s", str);
    }
}

void client_close(int sock) {
    close(sock);
}

int main() {
    int sock = client_connect(SERVER_IP, PORT);
    printf("Connected to server\n");
    client_process(sock);
    client_close(sock);
    printf("Disconnected from server\n");
}

