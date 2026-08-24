#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>


/**
 * @brief Creates an IPv4 UDP socket.
 *
 * @return The socket file descriptor, or -1 on error.
 */
int createSocket(void);


/**
 * @brief Closes a socket.
 *
 * @param socket_fd File descriptor of the socket to close.
 * @return 0 on success, or -1 on error.
 */
int closeSocket(int socket);

/**
 * @brief Binds an IPv4 UDP socket to a local address and port.
 *
 * @param socket File descriptor of the socket.
 * @param address IPv4 address written as text.
 * @param port Local port in host byte order.
 * @return 0 on success, or -1 on error.
 */

int bindSocket(int socket, const char* address, unsigned short int port);


/**
 *@brief wait the next msg send to socket and store it in the buffer and add \0 at the end
 *
 *@param socket file descriptor of the socket
 *@param buffer the buffer that will store the message
 *@param buffer_size his size
 *@return -1 error 0 msg empty int>0 the number of bytes received
 */

int receiveMsg(int socket, char *buffer, int buffer_size);


int sendMsg(int socket, char* msg, int msg_size, struct sockaddr *to, socklen_t tolen);
#endif