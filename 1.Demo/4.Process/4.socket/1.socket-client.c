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
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* 将 TEXT 的内容通过 SOCKET_FD 代表的套接字发送。*/
void write_text (int socket_fd, const char* text)
{
	/* 输出字符串(包含结尾的 NUL 字符)的长度。*/
	int length = strlen (text) + 1;
	write (socket_fd, &length, sizeof (length));
	/* 输出字符串。*/
	write (socket_fd, text, length);
}

int main (int argc, char* const argv[])
{
	const char* const socket_name = argv[1];
	const char* const message = argv[2];
	int socket_fd;
	struct sockaddr_un name;
	/* 创建套接字。*/
	socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
	/* 将服务器地址写入套接字地址结构中。*/
	name.sun_family = AF_LOCAL;
	strcpy (name.sun_path, socket_name);
	/* 连接套接字。*/
	connect (socket_fd, &name, SUN_LEN (&name));
	/* 将由命令行指定的文本信息写入套接字。*/
	write_text (socket_fd, message);
	close (socket_fd);
	return 0;
}
