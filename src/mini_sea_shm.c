#include "mini_sea_shm.h"

void * getshm(const char *keypath, size_t size)
{
    key_t shm = 0;
    int shmid = 0;
    void *base = NULL;

    shm = ftok(keypath, 0);
    if(shm < 0)
    {
        mlog("ftok fail [%s]",strerror(errno));
        return NULL;
    }

    shmid = shmget(shm, size, 0600 | IPC_CREAT);
    if(shmid < 0)
    {
        mlog("shmget fail[%s]", strerror(errno));
        return NULL;
    }

    base = shmat(shmid, NULL, 0);
    if(base == (void *)-1)
    {
        mlog("shmat fail[%s]", strerror(errno));
        return NULL;
    }

    return base;
}

/*
int main()
{

    mloginit("/home/wzx/Desktop");

    char *keypath = "/home/wzx/Desktop";
    size_t size = 1024;
    char *addr = getshm(keypath, size);

    return 0;
}
*/
