#ifndef _MINI_SEA_TASK_H_INCLUDE_
#define _MINI_SEA_TASK_H_INCLUDE_

extern void * rcvtask(int msgid, void *msg, size_t len);

extern int    dealtask(const char *src, int slen, char *dest, int dlen);

extern int    sndtask(int pair, int msgid, void *msg, size_t len);

#endif /*_MINI_SEA_TASK_H_INCLUDE_*/
