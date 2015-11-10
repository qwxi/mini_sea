#ifndef _MINI_SEA_FDPAIR_H_INCLUDE_
#define _MINI_SEA_FDPAIR_H_INCLUDE_

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#include "mini_sea_log.h"

extern int getfdpair(int *fd);

#endif /*_MINI_SEA_FDPAIR_H_INCLUDE_*/
