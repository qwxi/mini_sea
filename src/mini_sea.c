#include "mini_sea.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    char *blockkeypath = "/home/wzx/train/20151021/ipckeypath/blockkeypath";
    char *shmkeypath = "/home/wzx/train/20151021/ipckeypath/shm";
    char *queue_in_keypath = "/home/wzx/train/20151021/ipckeypath/queue_in";
    char *queue_out_keypath = "/home/wzx/train/20151021/ipckeypath/queue_out";

    void *shmaddr = NULL;
    int  msgid_in= 0;
    int  msgid_out= 0;
    int  sd = 0;

    data * block_head = malloc(sizeof(data));    
    memset(block_head, 0, sizeof(data));

    shmaddr = getshmaddr(blockkeypath, sizeof(timedata) * MINI_SEA_SD_COUNT); 
    if(shmaddr == (void *)-1) 
    {
        printf("func getshmaddr fail\n");
        return -1;
    }
    memset(shmaddr, 0, sizeof(timedata) * MINI_SEA_SD_COUNT);

    init(block_head, shmaddr, sizeof(timedata), MINI_SEA_SD_COUNT);
    

    shmaddr = getshmaddr(shmkeypath, sizeof(sdinfo) * MINI_SEA_SD_COUNT); 
    if(shmaddr == (void *)-1) 
    {
        printf("func getshmaddr fail\n");
        return -1;
    }
    memset(shmaddr, 0, sizeof(sdinfo) * MINI_SEA_SD_COUNT);

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

    sd = getsocket();
    if(sd < 0)
    {
        printf("func getsocket fail\n");
        return -1;
    }
 
    while( 1 )
    {
        ret = rcv_and_snd(shmaddr, msgid_in, msgid_out, sd, block_head);
        if(ret < 0)
        {
            printf("func rcv_and_snd fail\n"); 
        }
    }

    return 0;
}
