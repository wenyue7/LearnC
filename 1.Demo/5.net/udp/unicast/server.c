/*************************************************************************
    > File Name: server.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Fri Sep 15 09:06:19 2023
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

    int server_sockfd;
    int len;
    //服务器网络地址结构体
    struct sockaddr_in my_addr;
    //客户端网络地址结构体
    struct sockaddr_in remote_addr;
    int sin_size;
    //数据传送的缓冲区
    char buf[BUFSIZ];

    //数据初始化--清零
    memset(&my_addr, 0, sizeof(my_addr));
    //设置为IP通信
    my_addr.sin_family = AF_INET;
    //服务器IP地址--允许连接到所有本地地址上
    my_addr.sin_addr.s_addr = INADDR_ANY;
    //服务器端口号
    my_addr.sin_port = htons(8000);

    /*创建服务器端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if ((server_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket error");
        return 1;
    }

    /*将套接字绑定到服务器的网络地址上*/
    if (bind(server_sockfd, (struct sockaddr *)&my_addr,
             sizeof(struct sockaddr)) < 0) {
        perror("bind error");
        return 1;
    }
    sin_size = sizeof(struct sockaddr_in);
    printf("waiting for a packet...\n");

    /*接收客户端的数据并将其发送给客户端--recvfrom是无连接的*/
    if ((len = recvfrom(server_sockfd, buf, BUFSIZ, 0,
                        (struct sockaddr *)&remote_addr, &sin_size)) < 0) {
        perror("recvfrom error");
        return 1;
    }
    printf("received packet from %s:\n", inet_ntoa(remote_addr.sin_addr));
    buf[len] = '\0';
    printf("contents: %s\n", buf);

    /*关闭套接字*/
    close(server_sockfd);

    return 0;
}
