#ifndef _MINI_SEA_SOCKET_H_INCLUDE_
#define _MINI_SEA_SOCKET_H_INCLUDE_

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>

typedef uint16_t porttype;

extern int getserversocket(porttype port);

extern int getclientsocket(const char *ip, porttype port);

#endif /*_MINI_SEA_SOCKET_H_INCLUDE_*/
