#ifndef __CLIENT_H__
#define __CLIENT_H__

typedef struct {
    int socket;
    char* buffer_out;
    char* buffer_in;
} Client;

#endif