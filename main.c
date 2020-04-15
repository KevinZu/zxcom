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

int socket_fd = -1;

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

struct sockaddr_un srv_un = {0};

void *thread_run1(void* arg)
{
	char packet[100];
	int socketfd = *(int *)arg;
	char *param = "hello";

	while (1) {
		sleep(2); 
		ZxcomOnSendMsg(1,param,strlen(param),packet);
		if (sendto(socketfd,packet,strlen(param) + 4, 0,
			(struct sockaddr *)&srv_un, sizeof(srv_un)) == -1) {
			perror("send");
			close(socketfd);
			socketfd = -1;
			pthread_exit(NULL);
		}
	}
	printf("Sent successfully.\n");
	close(socketfd);
}


void *thread_run2(void* arg)
{
	int ret;
	int i =0;
	socklen_t addre_len;
	struct sockaddr_un remote_un = {0};	
	char buf[1024] = {0};
	int socket_fd = *(int *)arg;
	for(;;) {
		++i;
		memset(buf, 0, sizeof(buf));
		ret = recvfrom(socket_fd, buf, sizeof(buf), 0,
		        (struct sockaddr*)&remote_un, &addre_len);
		if (ret == -1) {
			perror("error when recvfrom, ");
			
		}
		ZxcomOnPacket(buf,ret);
		printf("ret:%d, %s\n", ret,buf);
		usleep(500000);
	}   

	close(socket_fd);
	socket_fd = -1;
}



int cmd1_handler(void *param)
{
	printf("handler param: %s\n",(char*)param);
	char *res = "go away";
	char packet[100];
	ZxcomOnSendResponse(1,res, strlen(res), packet);

	if(socket_fd < 0) {
		return -1;
	}

	if (sendto(socket_fd,packet,strlen(res) + 4, 0,
			(struct sockaddr *)&srv_un, sizeof(srv_un)) == -1) {
			perror("send");
			close(socket_fd);
			socket_fd = -1;
			pthread_exit(NULL);
	}
}

int main(int argc, char **argv)
{
	int ret;
	
	pthread_t tid_timer,tid_run1,tid_run2;

	char obuf[100];
	struct sockaddr_un cli_un = { 0 };
	srv_un.sun_family = AF_UNIX;
	strncpy(srv_un.sun_path, server_filename, sizeof(srv_un.sun_path));
	
	ZxcomInit();
	ZxcomAddCommand(1,cmd1_handler);
	ZxcomAddCommand(2,cmd1_handler);

	socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (socket_fd < 0) {
		printf("socket error %s errno: %d\n", strerror(errno), errno);
	}
	

	char mode = 's';
	if (argc > 1)
		mode = argv[1][0];
	if (mode == 's'){
		unlink(srv_un.sun_path);

		if (bind(socket_fd, (struct sockaddr *)&srv_un, sizeof(srv_un)) == -1) {
			printf("bind server err");
			exit(1);
		}

		pthread_create(&tid_run2,NULL,thread_run2,&socket_fd);
	} else {
		cli_un.sun_family = AF_UNIX;
		strncpy(cli_un.sun_path, client_filename, sizeof(cli_un.sun_path));
		unlink(cli_un.sun_path);
		
		if (bind(socket_fd, (struct sockaddr *)&cli_un, sizeof(cli_un)) == -1) {
			perror("bind client");
			exit(1);
		}
		
		pthread_create(&tid_run1,NULL,thread_run1,&socket_fd);
	}

	pthread_create(&tid_timer,NULL,thread_timer,NULL);

	while(1){
		sleep(10);
	}
	
	return 0;
}

