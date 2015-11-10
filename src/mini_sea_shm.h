#ifndef _MINI_SEA_SHM_H_INCLUDE_
#define _MINI_SEA_SHM_H_INCLUDE_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#include "mini_sea_log.h"

extern void * getshm(const char *keypath, size_t size);

#endif /*_MINI_SEA_SHM_H_INCLUDE_*/
