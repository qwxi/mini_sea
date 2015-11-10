#ifndef _MINI_SEA_SOCKET_H_INCLUDE_
#define _MINI_SEA_SOCKET_H_INCLUDE_

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "mini_sea_log.h"

typedef uint16_t porttype;

extern int getserversocket(porttype port);

extern int getclientsocket(const char *ip, porttype port);

extern int setsocketnonblocking(int sd);

#endif /*_MINI_SEA_SOCKET_H_INCLUDE_*/
