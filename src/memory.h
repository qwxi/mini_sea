#ifndef _MINI_SEA_MEMORY_H_INCLUDE_
#define _MINI_SEA_MEMORY_H_INCLUDE_

#include <linux/stddef.h>

#define BLOCKNUM 2000000

typedef struct{
    int cursor;
    void *list[BLOCKNUM];
}data;

extern void   init(data *head, void *addptr, int size, int num);
extern void * getone(data *head);
extern void   putone(data *head, void *ptr); 

#endif
