#include "mini_sea.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    char *shmkeypath = "/home/wzx/train/20151021/ipckeypath/shm";
    char *queuekeypath = "/home/wzx/train/20151021/ipckeypath/queue";

    void *shmaddr = NULL;
    int  msgid = 0;
    int  sd = 0;

    shmaddr = getshmaddr(shmkeypath, sizeof(sdinfo) * MINI_SEA_SD_COUNT);
    if(shmaddr == (void *)-1) 
    {
        printf("func getshmaddr fail\n");
        return -1;
    }
    memset(shmaddr, 0, sizeof(sdinfo) * MINI_SEA_SD_COUNT);

    msgid = getqueue(queuekeypath);
    if(msgid < 0) 
    {
        printf("func getqueue fail\n");
        return -1;
    }

    sd = getsocket();
    if(sd < 0)
    {
        printf("func getsocket fail\n");
        return -1;
    }
 
    while( 1 )
    {
        ret = rcv_and_snd(shmaddr, msgid, sd);
        if(ret < 0)
        {
            printf("func rcv_and_snd fail\n"); 
        }
    }

    return 0;
}
