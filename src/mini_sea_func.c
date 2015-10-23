#include "mini_sea.h"


void *getshmaddr(const char *shmkeypath, size_t size)
{
    
key_t shm = 0;
    shm = ftok(shmkeypath, 0);
    if(shm < 0)
    {
        printf("ftok fail [%s]",strerror(errno));
        return -1;
    }

int shmid = 0;
    shmid = shmget(shm, size, 0666 | IPC_CREAT);
    if(shmid < 0)
    {
        printf("shmget fail[%s]", strerror(errno));
        return -1;
    }

void *base = NULL;
    base = shmat(shmid, NULL, 0);
    if(base == (void *)-1)
    {
        printf("shmat fail[%s]", strerror(errno));
        return -1;
    }

    return base;
}

int getqueue(const char *queuekeypath)
{
key_t queue = 0;
    queue = ftok(queuekeypath, 0);
    if(queue < 0)
    {
        printf("ftok fail [%s]",strerror(errno));
        return -1;
    }

int msgid = 0;   
    msgid = msgget(msgkey, 0666 | IPC_CREAT);
    if(msgid < 0)
    {
        printf("msgget fail [%s]",strerror(errno));
        return -1;
    }
    
    return msgid;
}

int getsocket(void)
{
    int iRet = 0;
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family=AF_INET;
    saddr.sin_port = htons(MINI_SEA_SERVER_PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int sd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , 0);
    if(sd < 0)
    {
        printf("socket fail[%s]\n",strerror(errno));
        return -1;
    }

    iRet = bind(sd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if(iRet < 0)
    {
        printf("bind fail[%s]\n",strerror(errno));
        return -1;
    }

    iRet = listen(sd, 1024);
    if(iRet < 0)
    {
        printf("listen fail[%s]\n",strerror(errno));
        return -1;
    }
    return sd;
}

int rcv_and_snd(const char *shmaddr, int msgid, int sd)
{

    sdinfo *sdlist =  shmaddr;
    int ed = 0;
    int ret = 0;
    int write_count = 0;
    int nfds = 0;
    int n = 0;
    int fsd = 0;  /*used for loop sd*/
    msgbuf msg;

    struct epoll_event ev, events[MINI_SEA_EVENT_COUNT]; 

    ed = epoll_create(1);
    if(ed < 0)
    {
        printf("epoll_create fail[%s]\n",strerror(errno));
        return -1;
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sd;
    iRet = epoll_ctl(ed, EPOLL_CTL_ADD, sd, &ev);
    if(iRet < 0)
    {
        printf("epoll_ctl fail[%s]\n",strerror(errno));
        return -1;
    }

    for(;;)
    {
        nfds = epoll_wait(ed, events, MINI_SEA_EVENT_COUNT, -1);
        if (nfds == -1) {
            printf("epoll_wait fail[%s]\n",strerror(errno));
            return -1;
        }

        for (n = 0; n < nfds; ++n)
        {
            fsd = events[n].data.fd;
            if(sd == fsd)
            {
                for(;;)
                {
                    conn_sock = accept(sd, NULL, NULL);
                    if(conn_sock < 0)
                    {
                        if(errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            printf("accept fail [%s]\n", strerror(errno));
                            break;
                        }else{
                            printf("accept continue [%s]\n", strerror(errno));
                            continue;
                        }
                    }
                    ret = set_socket_non_blocking(conn_sock); 
                    if(ret < 0)
                    {
                        printf("set nonblockint sd[%d] fail\n", conn_sock);
                        close(conn_sock);
                        continue;
                    }
                    ev.events = EPOLLIN | EPOLLLET;
                    ev.data.fd = conn_sock;
                    ret = epoll_ctl(ed, EPOLL_CTL_ADD, conn_sock, &ev);
                    if(ret < 0)
                    {
                        printf("epoll_ctl fail[%s]\n", strerror(errno));
                        close(conn_sock);
                        continue;
                    }
                }/*for(;;)*/
            }/*sd == events[n].data.fd*/
            else if(events[n].events & EPOLLIN)
            {
                printf("meet event IN\n");
                ret = 0;
                for(;;)
                {      /*there is a big problem, */
                    ret = read(fsd, sdlist[fsd].read.buf+sdlist[fsd].read.len, \
                                    MINI_SEA_STR_LEN-sdlist[fsd].read.len);
                    if(ret < 0)
                    {
                        if(errno == EAGAIN || errno == EWOULDBLOCK)  
                        {
                            printf("read fail [%d:%s] and break\n", fsd, strerror(errno));
                            break;
                        }else{
                            printf("read fail [%d:%s] and continue\n", fsd, strerror(errno));
                            continue
                        }
                    }else{
                        sdlist[fsd].read.len+=ret;
                    }
                }
                msg.mtype = fsd;
                ret = msgsnd(msgid, (void)&msg, 0, IPC_NOWAIT); 
                if(ret < 0)
                {
                    printf("msgsnd fail [%d:%s]\n", fsd, strerror(errno));
                }
            }else if(events[n].events & EPOLLOUT)
            {
                write_count = 0;
                printf("meet event OUT\n");
                for(;;) 
                {
                    ret = write(fsd, sdlist[fsd].write.buf+write_count, sdlist[fsd].wirte.len); 
                    if(ret <  0)
                    {
                        if(errno == EAGAIN || errno ==  EWOULDBLOCK)
                        {
                            ev.events = EPOLLOUT | EPOLLET;
                            ev.data.fd = fsd;
                            ret = epoll_ctl(ed, EPOLL_CTL_MOD, fsd, &ev);
                            if(ret < 0)
                            {
                                 printf("epoll_ctl fail [%d:%s]\n", fsd, strerror(errno));
                            }
                            break; 
                         }
                         
                    }else{
                        sdlist[fsd].wirte.len-=ret;
                        write_count +=ret; 
                        if(sdlist[fsd].wirte.len == 0) break;
                    }
                }
            }else
            {
                if(events[n].events & EPOLLPRI)
                {
                    printf("meet event PRI\n");
                }else{
                    close(fsd);
                    printf("meet event %d and close sd[%s]\n", events[n].events, fsd);        
                }
            }
        }/*for (n = 0; n < nfds; ++n)*/


        while( 1 )
        {
            printf("Start while loop...\n");
            /*add code here*/
            ret = msgrcv(msgid, (void *)&msg, 0, IPC_NOWAIT); 
            if(ret < 0)
            {
                printf("msgrcv fail [%d:%s]\n", fsd, strerror(errno));
                break;
            }
            fsd = (int)msg.mtype; 
            write_count = 0;
            for(;;)
            {
                ret = write(fsd, sdlist[fsd].write.buf+write_count, sdlist[fsd].write.len);
                if(ret < 0)
                {
                     if(errno == EAGAIN || errno ==  EWOULDBLOCK)
                     {
                         ev.events = EPOLLOUT | EPOLLET;
                         ev.data.fd = fsd;
                         ret = epoll_ctl(ed, EPOLL_CTL_MOD, fsd, &ev);
                         if(ret < 0)
                         {
                             printf("epoll_ctl fail [%d:%s]\n", fsd, strerror(errno));
                         }
                         break; 
                     }
                }else{
                     sdlist[fsd].wirte.len-=ret;
                     write_count +=ret; 
                     if(sdlist[fsd].wirte.len == 0) break;
                }
            }
        }/*while( 1 )*/
    }


}


int set_socket_non_blocking(int sd)
{
   int flags = 0;
   int ret = 0;

   flags = fcntl (sd, F_GETFL, 0);
   if (flags == -1)
   {
      printf("fcntl fail [%s]\n", strerror(errno));
      return -1;
   }

   flags |= O_NONBLOCK;
   ret = fcntl (sd, F_SETFL, flags);
   if (ret == -1)
   {
      printf("fcntl fail [%s]\n", strerror(errno));
      return -1;
   }

   return 0;
    
}
