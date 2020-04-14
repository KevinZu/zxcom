#include <stdio.h>
#include "zxcom.h"
#include <pthread.h>

pthread_mutex_t g_mutex;


void ENTER_CRITICAL()
{
	pthread_mutex_lock(&g_mutex);
}


void EXIT_CRITICAL()
{
	pthread_mutex_unlock(&g_mutex);
}

int CRITICAL_INIT()
{
	pthread_mutex_init(&g_mutex, NULL);
}

int CRITICAL_DEINIT()
{
	pthread_mutex_destroy(&g_mutex);
}


int SendRequest(const char *req,const int len)
{
	printf("req: %s  len:%d\n",req,len);
	return 0;
}


int BuildRequest(const char *packet,const int packLen,char *req,unsigned int* pLen)
{
	
	return 0;
}


int main()
{
	ZxcomInit();
	
	return 0;
}
