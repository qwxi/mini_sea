#include "mini_sea_queue.h"

int  getqueue(const char *keypath)
{
    key_t queue = 0;
    int msgid = 0;

    queue = ftok(keypath, 0);
    if(queue < 0)
    {
        mlog("ftok fail [%s]",strerror(errno));
        return -1;
    }

    msgid = msgget(queue, 0600 | IPC_CREAT);
    if(msgid < 0)
    {
        mlog("msgget fail [%s]",strerror(errno));
        return -1;
    }

    return msgid;
}

/*
int main()
{
   mloginit("/home/wzx/Desktop");
 
   getqueue("/home/wzx");

   return 0;
}
*/
