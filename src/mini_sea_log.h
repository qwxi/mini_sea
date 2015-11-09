#ifndef _MINI_SEA_LOG_H_INCLUDE_
#define _MINI_SEA_LOG_H_INCLUDE_

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
/*
#include <time.h>
*/

#define LOGBUFLEN 1024
#define EXTRLEN   32

static char *logfilename;
static pid_t pid;

extern int   mloginit(const char *logfilename);

extern void  mlog(const char *fmt, ...);

#endif /*_MINI_SEA_LOG_H_INCLUDE_*/
