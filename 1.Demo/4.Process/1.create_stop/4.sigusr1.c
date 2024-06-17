/*
 * 以下代码中我们利用信号处理函数统计程序在运行期接收到 SIGUSR1 信号的次数。SIGUSR1 信号是一个为应用程序保留的信号。
 *
 * 对全局变量赋值可能也是不安全的,因为一个赋值操作可能由两个或更多机器指令完成,而在这些指令执行期间可能会有第二个信号到达,致使被修改的全局变量处于不完
 * 整的状态。如果你需要从信号处理函数中设置全局标志以记录信号的到达,这个标志必须是特殊类型 sig_atomic_t 的实例。Linux 保证对于这个类型变量的赋值操
 * 作只需要一条机器指令,因此不用担心可能在中途被打断。在 Linux 系统中,sig_atomic_t 就是基本的 int 类型;事实上,对 int 或者更小的整型变量以及指针
 * 赋值的操作都是原子操作。不过,如果你希望所写的程序可以向任何标准 UNIX 系统移植,则应将所有全局变量设为 sig_atomic_t 类型。
 */



#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigusr1_count = 0;
void handler (int signal_number)
{
	++sigusr1_count;
}
int main ()
{
	struct sigaction sa;
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &handler;
	sigaction (SIGUSR1, &sa, NULL);
	/* 这里可以执行一些长时间的工作。*/
	/* ... */
	printf ("SIGUSR1 was raised %d times\n", sigusr1_count);
	return 0;
}
