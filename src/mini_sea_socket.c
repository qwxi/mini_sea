#include "mini_sea_socket.h"

int  getserversocket(porttype port)
{

    int iRet = 0;
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family=AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int sd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , 0);
    if(sd < 0)
    {
        mlog("socket fail[%s]",strerror(errno));
        return -1;
    }

    iRet = bind(sd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if(iRet < 0)
    {
        mlog("bind fail[%s]",strerror(errno));
        return -1;
    }

    iRet = listen(sd, 1024);
    if(iRet < 0)
    {
        mlog("listen fail[%s]",strerror(errno));
        return -1;
    }
    return sd;

}

int getclientsocket(const char *ip, porttype port)
{
    return 0;
}

int setsocketnonblocking(int sd)
{
   int flags = 0;
   int ret = 0;

   flags = fcntl (sd, F_GETFL, 0);
   if (flags == -1)
   {
      mlog("fcntl fail [%s]", strerror(errno));
      return -1;
   }

   flags |= O_NONBLOCK;
   ret = fcntl (sd, F_SETFL, flags);
   if (ret == -1)
   {
      mlog("fcntl fail [%s]", strerror(errno));
      return -1;
   }

   return 0;

}


/*
#include <unistd.h>
int main()
{
    mloginit("/home/wzx/Desktop");
    int sd = getserversocket(9900);
    mlog("new sd[%d]", sd);

    sleep(200);
    return 0;
}
*/
