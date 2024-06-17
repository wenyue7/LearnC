/*
 * exec
 * Exec 族函数在名字和作用方面有细微的差别。
 *    名称包含 p 字母的函数(execvp 和 execlp)接受一个程序名作为参数,然后在当前的执行路径(译者注:环境变量 PATH 指明的路径)中搜索并执行这个
 * 程序;名字不包含 p 字母的函数在调用时必须指定程序的完整路径。
 *    名称包含 l 字母的函数(execl、execlp 和 execle)接收一个字符串数组作为调用程序的参数;这个数组必须以一个 NULL 指针作为结束的标志。名字
 * 包含 v字母的函数(execv, execvp 和 execve)以 C 语言中的 vargs 形式接受参数列表。
 *    名称包含 e 字母的函数(execve 和 execle)比其它版本多接收一个指明了环境变量列表的参数。这个参数的格式应为一个以 NULL 指针作为结束标记的
 * 字符串数组。每个字符串应该表示为“变量=值”的形式。
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* 产生一个新进程运行新的程序。PAORGAM 是要运行的程序的名字;系统会在执行路径中搜索这个程序运行。ARG_LIST 是一个以 NULL 指针结束的字符串列表,用作程序的参数列表。返回新进程的 ID。 */
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
	/* 准备传递给 ls 命令的参数列表 */
	char* arg_list[] = {
		/* argv[0], 程序的名称 */
		"ls",
		"-l",
		"/",
		NULL
	};
	/* 参数列表必须以 NULL 指针结束 */
	/* 建立一个新进程运行 ls 命令。忽略返回的进程 ID */
	spawn ("ls", arg_list);
	printf ("done with main program\n");
	return 0;
}
