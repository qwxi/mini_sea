#include "mini_sea_core.h"

int core(sdinfo *sdlist, int queue_in, int  queue_out, int sd, int readfd )
{

    int timeout = -1;
    int ed,ret,nfds,n,efd = 0;
    msgbuf msg;

    struct epoll_event ev, events[MINI_SEA_EVENT_COUNT];

    ed = epoll_create(1);
    if(ed < 0)
    {
        mlog("epoll_create fail[%s]",strerror(errno));
        return -1;
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sd;
    ret = epoll_ctl(ed, EPOLL_CTL_ADD, sd, &ev);
    if(ret < 0)
    {
        mlog("epoll_ctl fail[%s]",strerror(errno));
        return -1;
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = readfd;
    ret = epoll_ctl(ed, EPOLL_CTL_ADD, readfd, &ev);
    if(ret < 0)
    {
        mlog("epoll_ctl fail[%s]",strerror(errno));
        return -1;
    }

    for(;;)
    {
        mlog("Start to wait [%d]seconds event coming", timeout);

        nfds = epoll_wait(ed, events, MINI_SEA_EVENT_COUNT, timeout);
        if(nfds == -1){
            mlog("epoll_wait fail[%s]",strerror(errno));
            return -1;
        }

        mlog("Events[%d] return ", nfds);

        for (n = 0; n < nfds; ++n)
        {

            efd = events[n].data.fd;

            if(sd == efd){
/*****************************************/
                for(;;)
                {
                    int conn_sock = accept(sd, NULL, NULL);
                    if(conn_sock < 0)
                    {
                        if(errno == EAGAIN || errno == EWOULDBLOCK)
						{
						    mlog("accept fail [%s]", strerror(errno));
                            break;
                        }else{
                            mlog("accept continue [%s]", strerror(errno));
                            continue;
                        }
                    }

                    if(setsocketnonblocking(conn_sock) < 0)
                    {
                        mlog("set nonblockint sd[%d] fail", conn_sock);
                        close(conn_sock);
                        continue;
                    }

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = conn_sock;
                    if(epoll_ctl(ed, EPOLL_CTL_ADD, conn_sock, &ev) < 0 )
                    {
                        mlog("epoll_ctl fail[%s]", strerror(errno));
                        close(conn_sock);
                        continue;
                    }

                    mlog("new request socket [%d] incoming", conn_sock);

                }
/*****************************************/

            }else if(readfd == efd){

/****>>>>>>*******************************/

#define SIGBUFLEN 256
char sigbuf[SIGBUFLEN + 1];

                do{
                    ret = read(readfd, sigbuf, SIGBUFLEN);
                    if(ret < 0){
                        if(errno == EAGAIN || errno == EWOULDBLOCK) 
                        {
                            mlog("read fail [%d:%s] and break", readfd, strerror(errno));
                            break;
                        }else{
                            mlog("read fail [%d:%s] and continue", readfd, strerror(errno));
                            continue;
                        }
                    }
                    else if(ret == 0){
                         break;
                    }
                    else{
                         continue; 
                     }
                }while(1);


                while( 1 )
                {
                    mlog("Start while-loop to deal rsponse data...");

                    ret = msgrcv(queue_out, (void *)&msg, 0, 0, IPC_NOWAIT);
                    if(ret < 0)
                    {
                        mlog("msgrcv fail [%d:%s]", efd, strerror(errno));
                        break;
                    }
                    efd = (int)msg.mtype;

                    mlog("Rsponse data for fd[%d] from queue_out[%d]", efd, queue_out);

                    int write_count = 0;
                    for(;;)
                    {
                        ret = write(efd, sdlist[efd].write.buf+write_count, sdlist[efd].write.len);
                        if(ret < 0)
                        {
                            mlog("check other error---[%s] if need to break", strerror(errno));
                            if(errno == EPIPE) break;
                            if(errno == EAGAIN || errno ==  EWOULDBLOCK)
                            {
                                ev.events = EPOLLOUT | EPOLLET;
                                ev.data.fd = efd;
                                ret = epoll_ctl(ed, EPOLL_CTL_MOD, efd, &ev);
                                if(ret < 0)
                                {
                                    mlog("epoll_ctl fail [%d:%s]", efd, strerror(errno));
                                }
                                mlog("write full and fd[%d] recoming epoll", efd);
                                break;
                            }
                        }else{
                            sdlist[efd].write.len-=ret;
                            write_count +=ret;
                            if(sdlist[efd].write.len == 0)
                            {
                                mlog("rsponse data done");
                                break;
                            }
                        }
                    }
                }/*while( 1 )*/
/*****<<<<<<******************************/
                


                /*response data has incmonig*/
            }else if(events[n].events & EPOLLIN){

/****>>>>>>*******************************/
                for(;;)
                {    
                    ret = read(efd, sdlist[efd].read.buf+sdlist[efd].read.len, \
                                    MINI_SEA_STR_LEN-sdlist[efd].read.len);
                    if(ret < 0)
                    {
                        if(errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            mlog("read fail [%d:%s] and break", efd, strerror(errno));
                            break;
                        }else{
                            mlog("read fail [%d:%s] and continue", efd, strerror(errno));
                            continue;
                        }
                    }else if(ret == 0){
                        break;
                    }else{
                        sdlist[efd].read.len+=ret;
                    }
                }
                msg.mtype = efd;
                if(msgsnd(queue_in, (void *)&msg, 0, IPC_NOWAIT) < 0) /*need to control number (msgqueue > fd)*/
                {
                    mlog("msgsnd fail [%d:%s]", efd, strerror(errno));
                }

/*****<<<<<<******************************/

                /*deal in event*/
            }else if(events[n].events & EPOLLOUT){

/****>>>>>>*******************************/
                int write_count = 0;
                mlog("meet event OUT");
                for(;;)
                {
                    ret = write(efd, sdlist[efd].write.buf+write_count, sdlist[efd].write.len);
                    if(ret <  0)
                    {
                        if(errno == EAGAIN || errno ==  EWOULDBLOCK)
                        {
                            /**************************************************
                            ev.events = EPOLLOUT | EPOLLET;
                            ev.data.fd = efd;
                            ret = epoll_ctl(ed, EPOLL_CTL_MOD, efd, &ev);
                            if(ret < 0)
                            {
                                 mlog("epoll_ctl fail [%d:%s]", efd, strerror(errno));
                            }
                            ***************************************************/
                            break;
                         }

                    }else{
                        sdlist[efd].write.len-=ret;
                        write_count +=ret;
                        if(sdlist[efd].write.len == 0)
                        {
                            /*may be we need delete timedata in the timetree*/
                            break;
                        }
                    }
                }

/*****<<<<<<******************************/

                /*deal out event*/
            }else{

/****>>>>>>*******************************/
                if(events[n].events & EPOLLPRI)
                {
                    mlog("meet event PRI"); /*add code read pri data*/
                }else{
                    close(efd);
                    mlog("meet event %d and close sd[%d]", events[n].events, efd);
                } 
/*****<<<<<<******************************/

                /*deal exception*/
            }

        }
        
        dealsig();
    }

    return 0; 
}
