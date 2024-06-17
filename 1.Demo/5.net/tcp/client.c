/*************************************************************************
    > File Name: client.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Fri Sep 15 09:05:15 2023
 ************************************************************************/

#include <stdio.h> 
#include <string.h>
#include <unistd.h>
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
	//数据传送的缓冲区
	char buf[BUFSIZ];

	//数据初始化--清零
	memset(&remote_addr, 0, sizeof(remote_addr));
	//设置为IP通信，设置服务器IP地址、端口号
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote_addr.sin_port = htons(8000);

	/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
	//面向连接即:通信传输之前先打开一个连接,确认对方存在,连接被关闭时无法发送数据,
	//           Tcp是面向有连接型通信
	if ((client_sockfd = socket(PF_INET,SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		return 1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if (connect(client_sockfd, (struct sockaddr *)&remote_addr,
			    sizeof(struct sockaddr)) < 0) {
		perror("connect error");
		return 1;
	}
	printf("connected to server\n");
	//接收服务器端信息
	len = recv(client_sockfd, buf, BUFSIZ, 0);
	buf[len] = '\0'; 
	//打印服务器端信息
	printf("%s\n", buf);

	/*循环的发送接收信息并打印接收信息（可以按需发送）
	 * recv返回接收到的字节数，send返回发送的字节数*/
	while (1) {
		printf("Enter string to send:");
		scanf("%s", buf);
		if(!strcmp(buf, "quit"))
			break;
		len = send(client_sockfd, buf, strlen(buf), 0);
		len = recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len] = '\0';
		printf("received:%s\n", buf);
	}

	/*关闭套接字*/
	close(client_sockfd);

	return 0;
}
