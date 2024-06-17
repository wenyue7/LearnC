/*************************************************************************
    > File Name: client.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Fri Sep 15 09:05:15 2023
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

#define errlog(err) do{perror(err); exit(1);} while(0) 
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
	//加入到组播地址中
	struct ip_mreq mreq;
	
	//设定通信标准，基于UDP的广播发送
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		errlog("fail to socket");
	
	socklen_t mreq_len = sizeof(mreq);
	//组播地址
	mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
	//如果你只有一个网卡，本地IP地址可以填写 INADDR_ANY，
	//即从默认的网口接收组播数据包，如果你有不止一块网卡，并且它们连接了不同的网络，
	//你必须指定某个网卡的IP地址，从那个网络接口接收组播数据包。
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	//添加本地IP到组播地址中
	setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, mreq_len);
	
	//2、填充信息结构体
	groupcastaddr.sin_family = AF_INET;
	groupcastaddr.sin_port = htons(atoi(argv[2]));
	//必须将IP地址设置为 INADDR_ANY ，因为组播数据包的目的IP地址不确定，
	//是由IP地址与MAC地址组合而来，因此接收端必须有能力接收来自任何IP地址的数据包
	groupcastaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_aton("192.168.8.129", &groupcastaddr.sin_addr); 
	
	//3、绑定信息结构体
	if (bind(sockfd, (struct sockaddr*)&groupcastaddr, len) < 0) {
		errlog("fail to bind");
	}
	while (1) {
		//从局域网中接收数据
		recvfrom(sockfd, buf, N, 0, (struct sockaddr*)&groupcastaddr, &len);
		if (strncmp(buf, "quit", 4) == 0) {
			//setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, mreq_len);
			break;
		}
		printf("--> %s \n", buf);
	}
	
	close(sockfd);
	
	return 0;
}
