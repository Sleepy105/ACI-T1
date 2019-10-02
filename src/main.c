#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socket.h"

/* bool test_1 (void* server, void* client) {
    int res = client.write_multiple_regs(server);
    
}*/

int main(int argc, char const *argv[])
{
    printf("%d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("\t%s\n", argv[i]);
    }
    printf("Hello Sílvia\n");
    return 0;
}
