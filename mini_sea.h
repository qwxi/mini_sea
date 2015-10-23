#ifndef _MINI_SEA_H_INCLUDE__
#define _MINI_SEA_H_INCLUDE__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <unistd.h>

#define MINI_SEA_SERVER_PORT	9999
#define MINI_SEA_STR_LEN	1024*4
#define MINI_SEA_SD_COUNT	1024*10
#define MINI_SEA_EVENT_COUNT	256 *1

typedef struct{
    int len; 
    char buf[MINI_SEA_STR_LEN+1]; 
}str;

typedef struct{
    int sd;
    struct str read;
    struct str write;
}sdinfo;

typedef struct{
    long mtype;
}msgbuf;

extern void * getshmaddr(const char *shmkeypath, size_t size);
extern int    getqueue(const char *queuekeypath);
extern int    getsocket(void);
extern int    rcv_and_snd(const char *shmaddr, int msgid, int sd);
extern int    set_socket_non_blocking(int sd);

#endif  /*_MINI_SEA_H_INCLUDE_*/
