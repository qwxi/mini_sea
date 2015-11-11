#include "mini_sea_sig.h"

int flag = -99;

static void rcvsig(int signum)
{
    flag = signum;
    return;
}
void  dealsig(void)
{
    if(flag == -99)return;

    switch(flag)
    {
        case SIGALRM :
             mlog("Signal SIGALRM coming");
             break;
        case SIGINT :
             mlog("Signal SIGINT coming");
             break;
        case SIGIO :
             mlog("Signal SIGIO coming");
             break;
        case SIGSYS :
             mlog("Signal SIGSYS coming");
             break;
        case SIGCHLD :
             mlog("Signal SIGCHLD coming");
             break;
        case SIGPIPE :
             mlog("Signal SIGPIPE coming");
             break;
        default: 
             mlog("Unknown Signal %d coming", flag);
             break; 
    }

    flag = -99; 

    return;
}

void  configsig(void)
{

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    sigemptyset(&act.sa_mask);
    act.sa_handler = rcvsig; 

    sigaction(SIGALRM, &act, NULL);
    /*sigaction(SIGINT,  &act, NULL);*/
    sigaction(SIGIO,   &act, NULL);
    sigaction(SIGSYS,  &act, NULL);
    sigaction(SIGCHLD, &act, NULL);
    sigaction(SIGPIPE, &act, NULL);

    return;
}
