#include <sys/types.h>
#include <unistd.h>
#include "ucomlib.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "zxcom.h"
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
pthread_mutex_t g_mutex;

int server_fd = -1;
int client_fd = -1;


char * server_filename = "/tmp/socket-server";
char * client_filename = "/tmp/socket-client";

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

void *thread_timer(void* arg)
{
	//printf("new thread,thread is :%u,pid:%d\n",pthread_self(),getpid());
	while(1) {
		sleep(2);
		OnTimer();
	}
}

struct sockaddr_un srv_un_s = {0};
struct sockaddr_un srv_un_c = {0};


void *thread_run_c(void* arg)
{
	int ret;
	char packet[100],buf[100];
	socklen_t addre_len;
	int socketfd = *(int *)arg;
	char *param = "hello";
	//struct sockaddr_un srv_un = {0};	
	struct sockaddr_un remote_un = {0};	

	while (1) {
		sleep(2); 
		ZxcomOnSendMsg(1,param,strlen(param),packet);
		if (sendto(socketfd,packet,strlen(param) + 4, 0,
			(struct sockaddr *)&srv_un_s, sizeof(srv_un_s)) == -1) {
			perror("send");
			close(socketfd);
			socketfd = -1;
			pthread_exit(NULL);
		}

		memset(buf, 0, sizeof(buf));
		ret = recvfrom(socketfd, buf, sizeof(buf), 0,
		        (struct sockaddr*)&remote_un, &addre_len);
		if (ret == -1) {
			perror("error when recvfrom, ");
			continue;
			
		}
		ZxcomOnPacket(buf,ret);

		buf[ret] = 0;
		printf("cli ret:%d, contrl:0x%x    %s\n", ret,*(int*)buf,buf+4);
	}
}



void *thread_run_s(void* arg)
{
	int ret;
	int i =0;
	socklen_t addre_len;
	struct sockaddr_un remote_un = {0};	
	char buf[100] = {0};
	int server_fd = *(int *)arg;
	for(;;) {
		++i;
		memset(buf, 0, sizeof(buf));
		ret = recvfrom(server_fd, buf, sizeof(buf), 0,
		        (struct sockaddr*)&remote_un, &addre_len);
		if (ret == -1) {
			perror("error when recvfrom, ");
			continue;
			
		}
		ZxcomOnPacket(buf,ret);

		buf[ret] = 0;
		printf("ret:%d, contrl:0x%x    %s\n", ret,*(int*)buf,buf+4);
		usleep(500000);
	}   

	//close(server_fd);
	//server_fd = -1;
}





int cmd1_handler(void *param)   //server
{
	printf("handler param: %s\n",(char*)param);
	//struct sockaddr_un srv_un = {0};
	char *res = "go away";
	char packet[100];
	ZxcomOnSendResponse(1,res, strlen(res), packet);

	if(server_fd < 0) {
		return -1;
	}

	if (sendto(server_fd,packet,strlen(res) + 4, 0,
			(struct sockaddr *)&srv_un_c, sizeof(srv_un_c)) == -1) {
			perror("send");
			close(server_fd);
			server_fd = -1;
			pthread_exit(NULL);
	}
}


int response1_handler(void *param)
{
	printf("response handler param: %s\n",(char*)param);
}


int main(int argc, char **argv)
{
	int ret;
	
	pthread_t tid_timer,tid_run1,tid_run2;

	char obuf[100];
	//struct sockaddr_un srv_un_c = { 0 };
	

	
	ZxcomInit();
	ZxcomAddCommand(1,cmd1_handler);
	ZxcomAddResponse(1,response1_handler);

	srv_un_c.sun_family = AF_UNIX;
	strncpy(srv_un_c.sun_path, client_filename, sizeof(srv_un_c.sun_path));

	srv_un_s.sun_family = AF_UNIX;
	strncpy(srv_un_s.sun_path, server_filename, sizeof(srv_un_s.sun_path));		

	char mode = 's';
	if (argc > 1)
		mode = argv[1][0];
	if (mode == 's'){
		server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
		if (server_fd < 0) {
			printf("socket error %s errno: %d\n", strerror(errno), errno);
		}
	
		unlink(srv_un_s.sun_path);

		if (bind(server_fd, (struct sockaddr *)&srv_un_s, sizeof(srv_un_s)) == -1) {
			printf("bind server err");
			exit(1);
		}

		pthread_create(&tid_run2,NULL,thread_run_s,&server_fd);
	} else {
		client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
		if (client_fd < 0) {
			printf("socket error %s errno: %d\n", strerror(errno), errno);
		}
		unlink(srv_un_c.sun_path);
		
		if (bind(client_fd, (struct sockaddr *)&srv_un_c, sizeof(srv_un_c)) == -1) {
			perror("bind client");
			exit(1);
		}
		
		pthread_create(&tid_run1,NULL,thread_run_c,&client_fd);
	}

	pthread_create(&tid_timer,NULL,thread_timer,NULL);

	while(1){
		sleep(10);
	}
	
	return 0;
}

