#include "mini_sea_fdpair.h"

int getfdpair(int *fd)
{
    int ret = 0;
 
    ret =  socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    if(ret < 0) 
    {
        mlog("socketpair error [%s]", strerror(errno));
        return -1;
    }

    return 0;
}


/*
#include <unistd.h>
int main()
{
   int fd[2] = {0};

   getfdpair(fd);

   sleep(1000);
   return 0;
}
*/
