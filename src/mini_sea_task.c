#include "mini_sea.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    char *shmkeypath = "/home/wzx/train/20151021/ipckeypath/shm";
    char *queuekeypath = "/home/wzx/train/20151021/ipckeypath/queue";

    void *shmaddr = NULL;
    int  msgid = 0;
    int  fsd = 0;
    msgbuf msg;
    sdinfo *sdlist = NULL;

    shmaddr = getshmaddr(shmkeypath, sizeof(sdinfo) * MINI_SEA_SD_COUNT);
    if(shmaddr == (void *)-1) 
    {
        printf("func getshmaddr fail\n");
        return -1;
    }
    memset(shmaddr, 0, sizeof(sdinfo) * MINI_SEA_SD_COUNT);
    sdlist = shmaddr;

    msgid = getqueue(queuekeypath);
    if(msgid < 0) 
    {
        printf("func getqueue fail\n");
        return -1;
    }

    /*read data from msgqueueu*/       

    ret = msgrcv(msgid, (void *)&msg, 0, 0);
    if(ret < 0)
    {
        printf("msgrcv fail [%d:%s]\n", fsd, strerror(errno));
        return -1;
    }

    fsd = (int)msg.mtype;
    
    /*call before*/
    ret =  func(FUNC_BEFORE, &(sdlist[fsd]));
    if(ret < 0)
    {
        printf("call Func before fail\n");
    }

    /*call dobusiness*/
    ret =  func(FUNC_BUSINESS, &(sdlist[fsd]));
    if(ret < 0)
    {
        printf("call Func business fail\n");
    }

    /*call after*/
    ret =  func(FUNC_AFTER, &(sdlist[fsd]));
    if(ret < 0)
    {
        printf("call Func after fail\n");
    }

    /*write data into msgqueue*/
    ret = msgsnd(msgid, (void *)&msg, 0, 0);
    if(ret < 0) 
    {
        printf("msgsnd fail [%d:%s]\n", fsd, strerror(errno));
        return -1;
    }

    return 0;
}
