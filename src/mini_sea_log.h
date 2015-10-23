#ifndef _MINI_SEA_LOG_H_INCLUDE__
#define _MINI_SEA_LOG_H_INCLUDE__

typedef void (*log)(int level, char *content, int len );

extern void * getshmaddr(const char *shmkeypath, size_t size);
extern int    getqueue(const char *queuekeypath);
extern int    getsocket(void);
extern int    rcv_and_snd(const char *shmaddr, int msgid, int sd);
extern int    set_socket_non_blocking(int sd);

#endif  /*_MINI_SEA_LOG_H_INCLUDE_*/
