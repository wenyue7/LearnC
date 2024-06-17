/*************************************************************************
    > File Name: server.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Fri Sep 15 09:06:19 2023
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define errlog(err) \
	do {\
		perror(err);\
		exit(1);\
	} while(0)
#define N 32

int main(int argc, const char *argv[])
{
	(void)argc;
	(void)argv;

	int sockfd;
	//广播地址变量
	struct sockaddr_in groupcastaddr;
	socklen_t len = sizeof(groupcastaddr);
	char buf[N] = {0};
	
	//1、设定通信标准，基于UDP的广播发送
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		errlog("fail to socket");
	
	//2、填充网络信息结构体
	groupcastaddr.sin_family = AF_INET;
	//设置端口号
	groupcastaddr.sin_port = htons(atoi(argv[2]));
	//设置组播地址
	groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	while (1) {
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';
		
		//发送数据到组播地址中
		sendto(sockfd, buf, N, 0, (struct sockaddr*)&groupcastaddr, len);
	}
	
	close(sockfd);
	return 0;
}
