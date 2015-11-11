#ifndef _MINI_SEA_CORE_H_INCLUDE_
#define _MINI_SEA_CORE_H_INCLUDE_

#include <sys/epoll.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <sys/ipc.h>
#include <sys/msg.h>


#include "mini_sea_log.h"
#include "mini_sea_data.h"
#include "mini_sea_socket.h"
#include "mini_sea_sig.h"

extern int core(sdinfo *sdlist, int queue_in, int  queue_out, int sd, int readfd );

#endif /*_MINI_SEA_CORE_H_INCLUDE_*/
