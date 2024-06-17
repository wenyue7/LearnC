/*************************************************************************
    > File Name: main2.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Thu Sep 14 15:23:26 2023
 ************************************************************************/

#include <stdio.h>

// Most of the C compilers support a third parameter to main which
// store all envorinment variables
int main(int argc, char *argv[], char * envp[])
{
	(void)argc;
	(void)argv;
	int i;

	for (i = 0; envp[i] != NULL; i++)
		printf("\n%s", envp[i]);
	// 当程序调用getchar()时，程序就等着用户按键。
	// 用户输入的字符被存放在键盘缓冲区中，直到用户按回车为止(回车字符也放在缓冲区中)。
	// getchar();
	return 0;
}
