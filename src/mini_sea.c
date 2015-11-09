

int main(int argc, char *argv)
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



    char *shm_keypath = getenv("shm_keypath");
    if(shm_keypath == NULL)
    {
        mlog("getenv error [%s][suggest check evn shm_keypath]\n", strerror(errno));
        return -1;
    }

    char *queue_in_keypath = getenv("queue_in_keypath");
    if(queue_in_keypath == NULL)
    {
        mlog("getenv error [%s][suggest check evn queue_in_keypath]\n", strerror(errno));
        return -1;
    }

    char *queue_out_keypath = getenv("queue_out_keypath");
    if(queue_out_keypath == NULL)
    {
        mlog("getenv error [%s][suggest check evn queue_out_keypath]\n", strerror(errno));
        return -1;
    }

    shmaddr = getshm(shm_keypath, sizeof(sdinfo) * MINI_SEA_SD_COUNT);
    if(shmaddr == NULL)
    {
        mlog("getshm error [suggest os ipc-shm]\n");
        return -1;
    }

    int queue_in  = getqueue(queue_in_keypath); 
    if(queue_in < 0)
    {
        mlog("getqueue error [suggest os ipc-queue]\n");
        return -1;
    }


    int queue_out = getqueue(queue_out_keypath); 
    if(queue_out < 0)
    {
        mlog("getqueue error [suggest os ipc-queue]\n");
        return -1;
    }

    ret = getfdpair(fdpair);
    if(ret < 0)
    {
        mlog("getfdpair error [suggest check if fd of process%d is avalible]\n", getpid());
        return -1;
    }

    mlog("System[mini_sea] resourse has been configed\n");    

    pid_t pid = fork();
    if(pid < 0)
    {
        mlog();
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

        for(i = 0; i < TASKNUM; i++)
        {
             pid = fork();
             if(pid < 0)
             {

             }else if(pid == 0)
             {
                 mloginit(logdir);
                 break;
             }else{
                 
             }
        }

        mlog("Process_Task%d [%d] has started to work\n", i, getpid());

        while(1)         
        {
         sdinfo si = * rcvtask(int msgid, void *msg, size_t len);

         int    dealtask(const char *src, int slen, char *dest, int dlen);

         int    sndtask(int pair, int msgid, void *msg, size_t len);
             
        }



    }else{
         /*father process code*/
      
         sd = getsocket();
         if(sd < 0)
         {
             mlog("getsocket error [suggest check os port\n");
             return -1;
         }

         while(1)
         {
             ret = core(shmaddr, msgid_in, msgid_out, sd, );
             if(ret < 0)
             {
                 mlog("core error [sugget check log]\n");
             }
         }

    }



}
