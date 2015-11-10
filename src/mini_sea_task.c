#include "mini_sea_task.h"

int  rcvtask(int msgid, void *msg, size_t len, sdinfo *sdlist)
{
    int ret = 0;
    msgbuf *mb = (msgbuf *)msg;

    ret = msgrcv(msgid, msg, len, 0, 0);
    if(ret < 0)
    {
        mlog("msgrcv error [%s]", strerror(errno));
        return -1;
    }

    mlog("request [%s]", sdlist[(int)(mb->mtype)].read.buf); 

    ret = okcheck(sdlist[(int)(mb->mtype)].read.buf); 
    if(ret < 0)
    {
        mlog("request has not been integrated and continue to rcv");
        return -1;
    }

    return 0;
}

int  dealtask(sdinfo *sf)
{

    /*add business code here*/
    char buf[128] = {0};

    /*eg http*/
    sprintf(buf,  "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World",11);

    strcpy(sf->write.buf, buf); 
    sf->write.len = strlen(buf);

    return 0;
}

int  sndtask(int pair, int msgid, void *msg, size_t len)
{
    int ret = msgsnd(msgid, msg, len, 0);
    if(ret < 0)
    {
        mlog("msgsnd error [%s]", strerror(errno));
        return -1;
    }

#define SIGSTR "Z"
#define SIGSTRLEN sizeof(SIGSTR)-1

    while(write(pair, SIGSTR, SIGSTRLEN) !=  SIGSTRLEN)
    {
        mlog("send business data sig fail and continue by blocking [%s]", strerror(errno));
    }

    mlog("Deliver Rsponse Data OK");
    return 0; 
}

int  okcheck(char *buf)
{
    /*add code here to check data integrated because once recv may not get full req content*/
    return 0;
}


/* just complie ok
int main()
{
  
    return 0;
}
*/
