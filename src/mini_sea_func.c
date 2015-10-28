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
    int timeout = -1;
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
        nfds = epoll_wait(ed, events, MINI_SEA_EVENT_COUNT, timeout);
        if (nfds == -1) {
            printf("epoll_wait fail[%s]\n",strerror(errno));
            return -1;
        }

        for (n = 0; n < nfds; ++n)
        {
            fsd = events[n].data.fd;
            if(sd == fsd)
            {

struct timeval tv;
timedata *td = malloc(sizeof(timedata));
sdinfo  sdlink;
                sdlink.next = NULL;
                memset(&td, 0, sizeof(timedata));
                ret = gettimeofday(tv, NULL);
                if(ret < 0)
                {
                    printf("gettimeofday fail [%s]\n", strerror(errno));
                    return -1;
                }
                td->key = tv.tv_sec;
         
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
                    sdlist[conn_sock].sd = conn_sock; /*close fd as soon possible bye timeout*/ 
                    sdlist[coon_sock].next = NULL;
                    sdlink.next = &(sdlist[conn_sock]);
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
                if(sdlink.next != NULL)
                {
                    td.link = sdlink.next;
                    ret = timedata_insert(&timetree, td);
                    if(ret < 1)                  
                        printf("timedata_insert may be fail\n");
                }
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



            
struct timeval tv ;
timedata *td = NULL;
sdinfo   *sdptr;

            ret = gettimeofday(tv, NULL);
            if(ret < 0)
            {
                printf("gettimeofday fail [%s]\n", strerror(errno));
                return -1;
            }

            do{
                td = getmin(&timetree);         
                if(td == NULL)
                {
                    timeout = -1;
                    printf("getmin fail\n");
                    break;
                }
               
                sdptr = td->link; 
                if(sdptr == NULL)
                {
                    printf("sdptr is null\n");
                    return -1;
                }
  
                timeout = tv.tv_sec - td->key;              
                
                if(timeout >= MINI_SEA_TIMEOUT)
                {
                      do{
                           close(sdptr->sd);
                           sdptr->read.len = 0;
                           sdptr->wirte.len = 0;

                           sdptr = sdptr->next;
                      }while(sdptr != NULL)

                      timedata_delete(&timetree, td); 

                }else{
                     printf("wait [%d] seconds timeout\n", timeout);
                     break;
                }

            }while( 1 ) 

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

int timedata_delete(struct rb_root *root, timedata *ptr)
{
    rb_erase(&ptr->node, root);
    return 0;
}

timedata *getmin(struct rb_root *root)
{
    timedata *ptr = NULL;
    struct rb_node *node = root->rb_node;
    while(node->left != NULL){
        node = node->left; 
    }
    if(node != NULL) 
    {
        ptr = container_of(node, timedata, node); 
    }
    return ptr;
}

int  timedata_insert(struct rb_root *root, timedata *data)
{
        time_t result;
        struct rb_node **new = &(root->rb_node), *parent = NULL;

        /* Figure out where to put new node */
        while (*new) {
                timedata *this = container_of(*new, timedata, node);

                result = data->key - this->key;

                parent = *new;
                if (result < 0)
                        new = &((*new)->rb_left);
                else if (result > 0)
                        new = &((*new)->rb_right);
                else{
                        if(this->link == NULL)
                        {
                            this->link = data->link;
                        }else{
                            sdinfo *ptr = this->link;
                            while(ptr->next != NULL)
                            {
                                ptr = ptr->next;
                            }
                            ptr->next = data->link;
                        }
                        return 0;
                }
        }

        /* Add new node and rebalance tree. */
        rb_link_node(&data->node, parent, new);
        rb_insert_color(&data->node, root);

        return 1;
}
