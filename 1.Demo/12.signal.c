/*************************************************************************
    > File Name: 12.signal.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue May 14 14:11:20 2024
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signum)
{
    printf("Received signal %d, exiting...\n", signum);
    // 在这里执行清理工作
    exit(signum);
}

int main()
{
    // 注册SIGINT信号的处理函数
    signal(SIGINT, signal_handler);

    // 你的主程序逻辑
    while(1) {
        // ...
    }

    return 0;
}
