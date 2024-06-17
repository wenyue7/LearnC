#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main ()
{
	int fds[2];
	pid_t pid;
	/* 创建管道。标识管道两端的文件描述符会被放置在 fds 中。*/
	pipe (fds);
	/* 产生子进程。*/
	pid = fork ();
	if (pid == (pid_t) 0) {
		/* 这里是子进程。关闭我们的写入端描述符。*/
		close (fds[1]);
		/* 将读取端连接到标准输入*/
		dup2 (fds[0], STDIN_FILENO);
		/* 用 sort 替换子进程。*/
		execlp ("sort", "sort", 0);
	}
	else {
		/* 这是父进程。*/
		FILE* stream;
		/* 关闭我们的读取端描述符。*/
		close (fds[0]);
		/* 将写入端描述符转换为一个 FILE 对象,然后将信息写入。*/
		stream = fdopen (fds[1], "w");
		fprintf (stream, "This is a test.\n");
		fprintf (stream, "Hello, world.\n");
		fprintf (stream, "My dog has fleas.\n");
		fprintf (stream, "This program is great.\n");
		fprintf (stream, "One fish, two fish.\n");
		fflush (stream);
		close (fds[1]);
		/* 等待子进程结束。*/
		waitpid (pid, NULL, 0);
	}
	return 0;
}
