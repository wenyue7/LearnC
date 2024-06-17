/*************************************************************************
    > File Name: client.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Fri Sep 15 09:05:15 2023
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	int client_sockfd;
	int len;
	//服务器端网络地址结构体
	struct sockaddr_in remote_addr;
	int sin_size;
	//数据传送的缓冲区
	char buf[BUFSIZ];

	//数据初始化--清零
	memset(&remote_addr, 0, sizeof(remote_addr));
	//设置为IP通信、服务器IP地址、服务器端口号
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote_addr.sin_port = htons(8000);
	
	/*创建客户端套接字--IPv4协议，面向无连接通信，UDP协议*/
	//面量无连接即:无需确认对方是否存在,发送端随时发送数据,UDP是面向无连接型
	if ((client_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		return 1;
	}
	// 发送的内容
	strcpy(buf, "This is a test message");
	printf("sending: '%s'\n", buf);
	sin_size = sizeof(struct sockaddr_in);
	
	/*向服务器发送数据包*/
	if ((len = sendto(client_sockfd, buf, strlen(buf), 0,
					  (struct sockaddr *)&remote_addr,
					  sizeof(struct sockaddr))) < 0) {
		perror("recvfrom");
		return 1;
	}
	
	/*关闭套接字*/
	close(client_sockfd);
	
	return 0;
}
