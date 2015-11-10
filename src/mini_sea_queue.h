#ifndef _MINI_SEA_QUEUE_H_INCLUDE_
#define _MINI_SEA_QUEUE_H_INCLUDE_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>


extern int  getqueue(const char *keypath);

#endif /*_MINI_SEA_QUEUE_H_INCLUDE_*/
