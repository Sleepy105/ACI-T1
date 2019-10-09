#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdbool.h>
#include <stdio.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/** ERROR TYPES **/
#define SOCK_OK 0
#define SOCK_ERROR -1
#define SOCK_INVALID_ADDRESS -2
#define SOCK_VALUE_OUT_OF_BOUNDS -3


int socketCreate ();


/**
 * @brief Open a TCP Socket
 * 
 * @param address Address to connect to in string format
 * @param port Port to connect to
 * @return int OK >= 0
 */
int socketConnect (int socket, char* address, uint16_t port);

int socketBind (int socket, char* address, uint16_t port);

int socketListen (int socket, int backlog);

/**
 * @brief Close a TCP Socket
 * 
 * @param socket Socket to be closed
 * @return true An error has occured
 * @return false All went well
 */
bool socketClose (int socket);


/**
 * @brief Send 'buffer_size' bytes of 'buffer' to socket 'socket'. Returns the number sent or -1.
 * 
 * @param socket Socket to use
 * @param buffer Byte array with the data to be sent
 * @param buffer_size Number of bytes to send
 * @return int Number of bytes sent or -1 for Errors
 */
int socketWrite (int socket, void* buffer, size_t buffer_size);


/**
 * @brief Read 'buffer_size' bytes into 'buffer' from 'socket'. Returns the number read or -1 for errors.
 * 
 * @param socket Socket to read from
 * @param buffer Byte array with the data to be read
 * @param buffer_size Max size of the buffer
 * @return int Number of bytes sent or -1 for Errors
 */
int socketRead (int socket, void* buffer, size_t buffer_size);

#endif