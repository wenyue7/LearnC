#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* 将 COUNT 份 MESSAGE 的副本写入 STREAM,每次写入之后暂停 1 秒钟 */
void writer (const char* message, int count, FILE* stream)
{
	for (; count > 0; --count) {
		/* 将消息写入流,然后立刻发送 */
		fprintf (stream, "%s\n", message);
		fflush (stream);
		/* 休息,休息一会儿 */
		sleep (1);
	}
}

/* 从流中读取尽可能多的随机字符串 */
void reader (FILE* stream)
{
	char buffer[1024];
	/* 从流中读取直到流结束。 fgets 会不断读取直到遇见换行或文件结束符。 */
	while (!feof (stream)
			&& !ferror (stream)
			&& fgets (buffer, sizeof (buffer), stream) != NULL)
		fputs (buffer, stdout);
}

int main ()
{
	int fds[2];
	pid_t pid;
	/* 创建一个管道。代表管道两端的文件描述符将被放置在 fds 中。*/
	pipe (fds);
	/* 创建子进程。*/
	pid = fork ();
	if (pid == (pid_t) 0) {
		FILE* stream;
		/* 这里是子进程。关闭我们得到的写入端文件描述符副本。*/
		close (fds[1]);
		/* 将读取端文件描述符转换为一个 FILE 对象然后从中读取消息 */
		stream = fdopen (fds[0], "r");
		reader (stream);
		close (fds[0]);
	}
	else {
		/* 这是父进程。*/
		FILE* stream;
		/* 关闭我们的读取端文件描述符副本。*/
		close (fds[0]);
		/* 将写入端文件描述符转换为一个 FILE 对象然后写入数据。*/
		stream = fdopen (fds[1], "w");
		writer ("Hello, world.", 5, stream);
		close (fds[1]);
	}
	return 0;
}
