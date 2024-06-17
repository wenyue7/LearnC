/*
 * wait()函数用于使父进程（也就是调用wait()的进程）阻塞，直到一个子进程结束或者该进程接收到了一个指定的信号为止。
 * 如果该父进程没有子进程或者它的子进程已经结束，则wait()函数就会立即返回。
 * 
 * waitpid()的作用和wait()一样，但它并不一定要等待第一个终止的子进程（它可以指定需要等待终止的子进程），它还有
 * 若干选项，如可提供一个非阻塞版本的 wait()功能，也能支持作业控制。实际上，wait()函数只是 waitpid()函数的一
 * 个特例，在Linux 内部实现 wait()函数时直接调用的就是waitpid()函数。
 *
 * 你可以用 WIFEXITED 宏从一个子进程的返回状态中检测该进程是正常结束(利用exit 函数或者从 main 函数返回)还是被
 * 没有处理的信号异常终止。对于后一种情况,可以用 WTERMSIG 宏从中得到结束该进程的信号。
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int spawn (char* program, char** arg_list)
{
	pid_t child_pid;
	/* 复制当前进程。*/
	child_pid = fork ();
	if (child_pid != 0)
		/* 这里是父进程。*/
		return child_pid;
	else {
		/* 现在从执行路径中查找并执行 PROGRAM。*/
		execvp (program, arg_list);
		/* execvp 函数仅当出现错误的时候才返回。*/
		fprintf (stderr, "an error occurred in execvp\n");
		abort ();
	}
}

int main ()
{
	int child_status;
	/* 传递给 ls 命令的参数列表 */
	char* arg_list[] = {
		"ls", /* argv[0], 程序的名称 */
		"-l",
		"/",
		NULL /* 参数列表必须以 NULL 结束 */
	};
	/* 产生一个子进程运行 ls 命令。忽略返回的子进程 ID。*/
	spawn ("ls", arg_list);
	/* 等待子进程结束。*/
	wait (&child_status);
	if (WIFEXITED (child_status))
		printf ("the child proces exited normally, with exit code %d\n", WEXITSTATUS (child_status));
	else
		printf ("the child process exited abnormally\n");
	return 0;
}
