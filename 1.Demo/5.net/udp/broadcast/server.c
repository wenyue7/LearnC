/*************************************************************************
    > File Name: server.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Fri Sep 15 09:06:19 2023
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define TTL 64
#define BUFF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	//sender的socket
	int sender_socket;
	//广播地址
	struct sockaddr_in broadcast_addr;
	//文件指针
	FILE *fp;
	//字符缓冲
	char buff[BUFF_SIZE];
	//用于配置socket参数
	int opt_so_broadcast = 1;

	if (argc != 3) {
		printf("Uasge : %s <GroupIP> <PORT> \n", argv[0]);
		exit(1);
	}
	
	//初始化socket
	sender_socket = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&broadcast_addr, 0, sizeof(broadcast_addr));
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_addr.s_addr = inet_addr(argv[1]);
	broadcast_addr.sin_port = htons(atoi(argv[2]));
	
	//设置socket可选项，因为默认生成的会阻止广播，所以要将可选项中的SO_BROADCAST标志置为1
	setsockopt(sender_socket, SOL_SOCKET, SO_BROADCAST, (void *)&opt_so_broadcast,
			   sizeof(opt_so_broadcast));
	//打开文件
	if ((fp = fopen("word_file.txt","r")) == NULL)
		error_handling("fopen() error");
	
	while (!(feof(fp))) {
		//从文件中读取数据
		fgets(buff, BUFF_SIZE, fp);
		printf("%s", buff);
		//将数据发送到多播组
		sendto(sender_socket, buff, strlen(buff), 0, (struct sockaddr *)&broadcast_addr,
			   sizeof(broadcast_addr));
		sleep(2);
	}
	
	fclose(fp);
	close(sender_socket);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
