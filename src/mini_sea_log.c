#include <mini_sea_log.h>


void mlog(const char* ms, ... )
{
	char Log[1024] = {0};
	char buffer[1024] = {0};
	va_list args;
	va_start(args, ms);
	vsprintf( Log ,ms,args);
	va_end(args);

	time_t now;
	time(&now);
	struct tm *local;
	local = localtime(&now);

    
    Log[1000] = '\0';
	sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,
				local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
				Log);
    
	FILE* file = fopen("Running.log","a+");
	fwrite(buffer,1,strlen(buffer),file);
	fclose(file);

	return ;
}

/*
int main()
{
	mlog("hello world%d\n", 123);
	return 0;
}
*/
