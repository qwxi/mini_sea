#include "mini_sea.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    char *shmkeypath = "/home/wzx/train/20151021/ipckeypath/shm";
    char *queue_in_keypath = "/home/wzx/train/20151021/ipckeypath/queue_in";
    char *queue_out_keypath = "/home/wzx/train/20151021/ipckeypath/queue_out";

    void *shmaddr = NULL;
    int  msgid_in, msgid_out = 0;
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

    msgid_in = getqueue(queue_in_keypath);
    if(msgid_in < 0) 
    {
        printf("func getqueue fail\n");
        return -1;
    }

    msgid_out = getqueue(queue_out_keypath);
    if(msgid_out < 0) 
    {
        printf("func getqueue fail\n");
        return -1;
    }

while( 1 )   
{

    /*read data from msgqueueu*/       

    ret = msgrcv(msgid_in, (void *)&msg, 0, 0, 0);
    if(ret < 0)
    {
        printf("msgrcv fail [%d:%s]\n", fsd, strerror(errno));
        return -1;
    }

    fsd = (int)msg.mtype;
    
    
    sdlist[fsd].read.buf[sdlist[fsd].read.len] = '\0';
    printf("request:[%s]\n", sdlist[fsd].read.buf);
     
    sprintf(sdlist[fsd].write.buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World",11);
    sdlist[fsd].write.len = strlen(sdlist[fsd].write.buf);
    
/*
    ret =  func(FUNC_BEFORE, &(sdlist[fsd]));
    if(ret < 0)
    {
        printf("call Func before fail\n");
    }

    ret =  func(FUNC_BUSINESS, &(sdlist[fsd]));
    if(ret < 0)
    {
        printf("call Func business fail\n");
    }

    ret =  func(FUNC_AFTER, &(sdlist[fsd]));
    if(ret < 0)
    {
        printf("call Func after fail\n");
    }
*/

    /*write data into msgqueue*/
    ret = msgsnd(msgid_out, (void *)&msg, 0, 0);
    if(ret < 0) 
    {
        printf("msgsnd fail [%d:%s]\n", fsd, strerror(errno));
        return -1;
    }

}

    return 0;
}
