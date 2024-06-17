/*
 * 不要直接修改 environ 变量;如果需要修改环境变量,则应通过 setenv 和 unsetenv 函数完成。
 * 通常,当启动一个新程序的时候,这个程序会从调用者那里继承一份运行环境(在交互运行的情况下,
 * 通常调用者是 shell 程序)。因此,你从 shell 中运行的程序可以使用你通过shell 设置的环境变量。
 *
 */
#include <stdio.h>
/* ENVIRON 变量包含了当前整个环境。*/
extern char** environ;
int main ()
{
	char** var;
	for (var = environ; *var != NULL; ++var)
		printf ("%s\n", *var);
	return 0;
}
