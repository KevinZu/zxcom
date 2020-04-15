#include <sys/types.h>
#include <unistd.h>
#include "ucomlib.h"
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include "zxcom.h"
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>

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

void *thread_timer(void* arg)
{
	printf("new thread,thread is :%u,pid:%d\n",pthread_self(),getpid());
	while(1) {
		sleep(2);
		OnTimer();
	}
}


void *thread_run1(void* arg)
{
	char packet[100];
	int socketfd = *(int *)arg;
	char *param = "hello";
	while(1) {
		sleep(2);
		ZxcomOnSendMsg(1,param,strlen(param),packet);
		if((send(socketfd, packet,strlen(param) + 4, 0)) < 0) {
			printf("send message error: %s errno : %d", strerror(errno), errno);
			close(socketfd);
			socketfd = -1;
			pthread_exit(NULL);
		}
	}
}

int conn_fd = -1;

void *thread_run2(void* arg)
{
	int recv_len = -1;
	
	char buff[100];
	
	int socket_fd = *(int *)arg;
	for(;;) {
			conn_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL);
			if(conn_fd < 0) {
				printf("accpet socket error: %s errno :%d\n", strerror(errno), errno);
				continue;
			}
			while(1){
				recv_len = recv(conn_fd, buff, 100, 0);
				if (recv_len < 0) {
					printf( "recv error %s errno: %d\n", strerror(errno), errno);
					break;
				} else if(recv_len == 0){
					continue;
				}

				buff[recv_len] = '\0';
				printf( "recv message from client: %s\n", buff);
				ZxcomOnPacket(buff,recv_len);
			}
			close(conn_fd);
			conn_fd = -1;
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

	if(conn_fd != -1){
		send(conn_fd,packet,strlen(res) + 1,0);
	}
	
}

int main(int argc, char **argv)
{
	int ret;
	
	pthread_t tid_timer,tid_run1,tid_run2;
	
	char *server_ip_addr = "127.0.0.1";
	int server_ip_port = 10004;
	struct sockaddr_in t_sockaddr;

	ZxcomInit();
	ZxcomAddCommand(1,cmd1_handler);
	ZxcomAddCommand(2,cmd1_handler);

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		printf("socket error %s errno: %d\n", strerror(errno), errno);
	}

	char mode = 's';
	if (argc > 1)
		mode = argv[1][0];
	if (mode == 's'){
		ret = bind(socket_fd,(struct sockaddr *) &t_sockaddr,sizeof(t_sockaddr));
		if (ret < 0) {
			printf("bind socket error %s errno: %d\n", strerror(errno), errno);
		}

		ret = listen(socket_fd, 1024);
		if (ret < 0) {
			printf("listen error %s errno: %d\n", strerror(errno), errno);
		}

		pthread_create(&tid_run2,NULL,thread_run2,&socket_fd);
	} else {
		memset(&t_sockaddr, 0, sizeof(struct sockaddr_in));
		t_sockaddr.sin_family = AF_INET;
		t_sockaddr.sin_port = htons(server_ip_port);
		inet_pton(AF_INET, server_ip_addr, &t_sockaddr.sin_addr);
		while(1){
			if((connect(socket_fd, (struct sockaddr*)&t_sockaddr, sizeof(struct sockaddr))) < 0 ) {
				printf("connect error %s errno: %d\n", strerror(errno), errno);
				sleep(5);
				continue;
			}
			break;
		}
		
		pthread_create(&tid_run1,NULL,thread_run1,&socket_fd);
	}

	pthread_create(&tid_timer,NULL,thread_timer,NULL);

	while(1){
		sleep(10);
	}
	
	return 0;
}

