#include <stdio.h>
#include <stdlib.h>

#include "mini_sea_data.h"
#include "mini_sea_fdpair.h"
#include "mini_sea_log.h"
#include "mini_sea_queue.h"
#include "mini_sea_shm.h"
#include "mini_sea_socket.h"
#include "mini_sea_task.h"
#include "mini_sea_core.h"
#include "mini_sea_sig.h"

int main(int argc, char *argv[])
{

    int fdpair[2];
    int ret = 0;
    int readfd = 0;
    int writefd = 0;

    char *logdir = getenv("logdir");
    if(logdir == NULL)
    {
        printf("getenv error [env:logdir not config]\n"); 
        return -1;
    }
    /*child process recall mloginit to reset pid*/
    ret = mloginit(logdir); 
    if(ret < 0)
    {
        printf("mloginit error\n"); 
        return -1;
    }

    mlog("System[mini_sea] start to init resourse");    

    char *shm_keypath = getenv("shm_keypath");
    if(shm_keypath == NULL)
    {
        mlog("getenv error [%s][suggest check evn shm_keypath]", strerror(errno));
        return -1;
    }

    char *queue_in_keypath = getenv("queue_in_keypath");
    if(queue_in_keypath == NULL)
    {
        mlog("getenv error [%s][suggest check evn queue_in_keypath]", strerror(errno));
        return -1;
    }

    char *queue_out_keypath = getenv("queue_out_keypath");
    if(queue_out_keypath == NULL)
    {
        mlog("getenv error [%s][suggest check evn queue_out_keypath]", strerror(errno));
        return -1;
    }

    sdinfo *sdlist = (sdinfo *) getshm(shm_keypath, sizeof(sdinfo) * MINI_SEA_SD_COUNT);
    if(sdlist == NULL)
    {
        mlog("getshm error [suggest os ipc-shm]");
        return -1;
    }

    int queue_in  = getqueue(queue_in_keypath); 
    if(queue_in < 0)
    {
        mlog("getqueue error [suggest os ipc-queue]");
        return -1;
    }


    int queue_out = getqueue(queue_out_keypath); 
    if(queue_out < 0)
    {
        mlog("getqueue error [suggest os ipc-queue]");
        return -1;
    }

    ret = getfdpair(fdpair);
    if(ret < 0)
    {
        mlog("getfdpair error [suggest check if fd of process%d is avalible]", getpid());
        return -1;
    }
    
    configsig();

    mlog("System[mini_sea] resourse has been configed!!!");    

int sun = 0;

    pid_t pid = fork();
    if(pid < 0)
    {
        mlog("fork error [%s] and mini_sea exit", strerror(errno));
        return -1;
    }else if(pid == 0)  /*child process*/
    {
        writefd = fdpair[1];
        close(fdpair[0]);
        sun = 1;

    }else{ /*father process*/

        readfd = fdpair[0];
        close(fdpair[1]);
        sun = 0;
    }

    if(sun) 
    {
         /*child process code*/

#define TASKNUM 2
int i = 0;

        for(i = 0; i < TASKNUM; i++)
        {
             pid = fork();
             if(pid < 0)
             {

             }else if(pid == 0)
             {
                 break;
             }else{
                 
             }
        }

        mloginit(logdir);
        mlog("Process_Task%d [%d] has started to work", i, getpid());

        while(1)         
        {
msgbuf msg;
             ret = rcvtask(queue_in, &msg, 0, sdlist);
             if(ret < 0)
             {
                 mlog("rcvtask error and continue");
                 continue;
             }

             ret = dealtask(&(sdlist[(int)msg.mtype]));
             if(ret < 0)
             {
                 mlog("dealtask error and continue");
                 continue;
             }

             ret = sndtask(writefd, queue_out, &msg, 0);
             if(ret < 0)
             {
                 mlog("sndtask error and continue");
                 continue;
             }
        }

    }else{
         /*father process code*/

         int sd = getserversocket(atoi(getenv("SERVER_PORT")));
         if(sd < 0)
         {
             mlog("getserversocket error [suggest check os port");
             return -1;
         }

         while(1)
         {
             if(core(sdlist, queue_in, queue_out, sd, readfd) < 0)
             {
                 mlog("core error [sugget check log]");
             }
         }

    }



}
