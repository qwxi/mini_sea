#include "mini_sea_log.h"

char *logfilename = NULL;
pid_t pid = 0;

int   mloginit(const char *logdir)
{
     pid = getpid();
     logfilename = malloc(strlen(logdir)+64);
     if(logfilename == NULL)
     {
         printf("malloc error [%s]\n", strerror(errno));
         return -1;
     }
     sprintf(logfilename, "%s/%d", logdir, pid); 
     return 0;
}

void  mlog(const char *fmt, ...)
{
     if(logfilename == NULL || pid == 0) return;

     char log[LOGBUFLEN+1]  = {0}; 
     char buf[LOGBUFLEN+EXTRLEN]  = {0}; 

     va_list args;
     va_start(args, fmt);
     vsnprintf(log, LOGBUFLEN, fmt, args);
     va_end(args);

     struct timeval tv ;
     gettimeofday(&tv, NULL);
/*
     time_t now;
     struct tm *local = localtime(&now); 

     sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d %s\n",  \
                local->tm_year+1900,  \
                local->tm_mon,        \
                local->tm_mday,       \
                local->tm_hour,       \
                local->tm_min,        \
                local->tm_sec, log); 
*/
     sprintf(buf, "\n%ld:%ld %d [%s]", tv.tv_sec, tv.tv_usec, pid, log);

     int fd = open(logfilename, O_RDWR | O_CREAT | O_APPEND , S_IRWXU );  
     write(fd, buf, LOGBUFLEN + EXTRLEN);
     close(fd);

     return ;
}


/*
int main()
{
    
    int ret =  mloginit("/home/wzx/Desktop");
    int i ;
  
    for(i = 0; i < 10000; i++)
    {
        mlog("log func test %d---%d", getpid, i);    
    }

    return 0;
}
*/
