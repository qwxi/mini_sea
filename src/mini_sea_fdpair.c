#include "mini_sea_fdpair.h"

int getfdpair(int *fd)
{
 
    if(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0)
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
