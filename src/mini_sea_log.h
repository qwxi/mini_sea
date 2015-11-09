#ifndef _MINI_SEA_LOG_H_INCLUDE_
#define _MINI_SEA_LOG_H_INCLUDE_

#define LOGBUFLEN 1024

static char *logfilename;
static pid_t pid;

extern int   mloginit(const char *logfilename);

extern void  mlog(const char *fmt, ...);

#endif /*_MINI_SEA_LOG_H_INCLUDE_*/
