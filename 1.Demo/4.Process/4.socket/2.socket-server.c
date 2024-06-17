/*
 * 要尝试这个例子,应在一个窗口中运行服务端程序。指定一个套接字文件的路径——例如 /tmp/socket 作为参数:
 * $ ./socket-server /tmp/socket
 * 在另一个窗口中指明同一个套接字和消息,并多次运行客户端程序。
 * $ ./socket-client /tmp/socket “Hello, world.”
 * $ ./socket-client /tmp/socket “This is a test.”
 * 服务端将接收并输出这些消息。要关闭服务端程序,从客户端发送“ quit ”即可:
 * $ ./socket-client /tmp/socket “quit”
 * 这样服务端程序就会退出。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* 不断从套接字读取并输出文本信息直到套接字关闭。当客户端发送"quit"消息的时候返回非 0 值,否则返回 0。*/

int server (int client_socket)
{
	while (1) {
		int length;
		char* text;
		/* 首先,从套接字中获取消息的长度。如果 read 返回 0 则说明客户端关闭了连接。*/
		
		if (read (client_socket, &length, sizeof (length)) == 0)
			return 0;
		/* 分配用于保存信息的缓冲区。*/
		text = (char*) malloc (length);
		/* 读取并输出信息。*/
		read (client_socket, text, length);
		printf ("%s\n", text);
		/* 如果客户消息是"quit",我们的任务就此结束。*/
		if (!strcmp (text, "quit")) {
			/* 释放缓冲区。*/
			free (text);
			return 1;
		}
		/* 释放缓冲区。*/
		free (text);
		/* 译者注:合并了勘误中的提示,并增加此返回语句。*/
		return 0;
	}
}

int main (int argc, char* const argv[])
{
	const char* const socket_name = argv[1];
	int socket_fd;
	struct sockaddr_un name;
	int client_sent_quit_message;
	/* 创建套接字。*/
	socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
	/* 指明这是服务器。*/
	name.sun_family = AF_LOCAL;
	strcpy (name.sun_path, socket_name);
	bind (socket_fd, &name, SUN_LEN (&name));
	/* 监听连接。*/
	listen (socket_fd, 5);
	/* 不断接受连接,每次都调用 server() 处理客户连接。直到客户发送"quit"消息的时候退出。*/
	do {
		struct sockaddr_un client_name;
		socklen_t client_name_len;
		int client_socket_fd;
		/* 接受连接。*/
		client_socket_fd = accept(socket_fd, &client_name, &client_name_len);
		/* 处理连接。*/
		client_sent_quit_message = server (client_socket_fd);
		/* 关闭服务器端连接到客户端的套接字。*/
		close (client_socket_fd);
	}
	while (!client_sent_quit_message);
	/* 删除套接字文件。*/
	close (socket_fd);
	unlink (socket_name);
	return 0;
}
