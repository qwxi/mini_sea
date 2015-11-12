#ifndef _MINI_SEA_TASK_H_INCLUDE_
#define _MINI_SEA_TASK_H_INCLUDE_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "mini_sea_log.h"

#include "mini_sea_data.h"

extern int  rcvtask(int msgid, void *msg, size_t len, sdinfo *sf);

extern int  dealtask(sdinfo *sf);
extern int  dealtask2(sdinfo *sf);

extern int  sndtask(int pair, int msgid, void *msg, size_t len);

extern int  okcheck(char *buf);


#endif /*_MINI_SEA_TASK_H_INCLUDE_*/
