#ifndef _MINI_SEA_DATA_H_INCLUDE_
#define _MINI_SEA_DATA_H_INCLUDE_


#define MINI_SEA_STR_LEN    	1024*4
#define MINI_SEA_SD_COUNT   	1024*10
#define MINI_SEA_EVENT_COUNT    256 *1


typedef struct{
    int len;
    char buf[MINI_SEA_STR_LEN+1];
}str;

typedef struct sdinfo sdinfo;
struct sdinfo{
    int sd;
    str read;
    str write;
};

typedef struct{
    long mtype;
}msgbuf;


#endif /*_MINI_SEA_DATA_H_INCLUDE_*/
