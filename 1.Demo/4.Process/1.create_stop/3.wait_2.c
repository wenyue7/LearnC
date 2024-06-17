/*
 * 在父进程中立刻调用 wait 进行等待并没有什么问题,只是会导致父进程阻塞等待子进程结束。但是,很多时候你希望
 * 在子进程运行的同时,父进程继续并行运行。怎么才能保证能清理已经结束运行的子进程而不留下任何僵尸进程在系统
 * 中浪费资源呢?
 *
 * 一种解决方法是让父进程定期调用 wait3 或 wait4 以清理僵尸子进程。在这种情况调用 wait 并不合适,因为如
 * 果没有子进程结束,这个调用会阻塞直到子进程结束为止。然而,你可以传递 WNOHANG 标志给 wait3 或 wait4 函
 * 数作为一个额外的参数。如果设定了这个标志,这两个函数将会以非阻塞模式运行——如果有结束的子进程,它们会进行清
 * 理;否则会立刻返回。第一种情况下返回值是结束的子进程 ID,否则返回 0。
 *
 * 另外一种更漂亮的解决方法是当一个子进程结束的时候通知父进程。也就是利用“进程间通信”
 *
 * Linux 利用信号机制替你完成了通信的工作。当一个子进程结束的时候,Linux 给父进程发送 SIGCHLD 信号。这个信号
 * 的默认处理方式是什么都不做;这也许是为什么之前你忽略了它的原因。
 */

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

sig_atomic_t child_exit_status;
void clean_up_child_process (int signal_number)
{
	/* 清理子进程。*/
	int status;
	wait (&status);
	/* 在全局变量中存储子进程的退出代码。*/
	child_exit_status = status;
}

int main ()
{
	/* 用 clean_up_child_process 函数处理 SIGCHLD。*/
	struct sigaction sigchild_action;
	memset (&sigchild_action, 0, sizeof (sigchild_action));
	sigchild_action.sa_handler = &clean_up_child_process;
	sigaction (SIGCHLD, &sigchild_action, NULL);
	/* 现在进行其它工作,包括创建一个子进程。*/
	/* ... */
	return 0;
}
