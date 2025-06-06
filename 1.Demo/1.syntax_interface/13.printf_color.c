/*************************************************************************
    > File Name: 13.printf_color.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Thu Nov 30 16:46:10 2023
 ************************************************************************/

#include <stdio.h>

// 如果想要输出不同的颜色进行区分，就需要用到printf的控制命令：\033[m。
// 控制命令以\033[开头，以m结尾，而中间则是属性码，属性代码之间使用;分隔，
// 如\033[1;34;42m。而属性代码的含义见下面的表格。

// 通用格式控制               前景色               背景色
// 属性代码	功能              属性代码 颜色        属性代码	 颜色
// 0        重置所有属性      30       黑色        40	     黑色
// 1        高亮/加粗         31       红色        41	     红色
// 2        暗淡              32       绿色        42	     绿色
// 4        下划线            33       黄色        43	     黄色
// 5        闪烁              34       蓝色        44	     蓝色
// 7        反转              35       品红        45	     品红
// 8        隐藏              36       青色        46	     青色

// 更多命令可参考： https://en.wikipedia.org/wiki/ANSI_escape_code


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    printf("\033[0m\033[1;31m%s\033[0m\n", "hello world");
    printf("\033[0m\033[1;32m%s\033[0m\n", "hello world");
    printf("\033[0m\033[1;33m%s\033[0m\n", "hello world");
    printf("\033[0m\033[1;34m%s\033[0m\n", "hello world");
    printf("\033[0m\033[1;35m%s\033[0m\n", "hello world");
    printf("\033[0m\033[1;36m%s\033[0m\n", "hello world");
}
