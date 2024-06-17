/*************************************************************************
    > File Name: pipe_cmdline.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed 10 Apr 2024 03:11:43 PM CST
 ************************************************************************/

// usage: echo "abc" | ./pipe_cmdline

#include <stdio.h>

#define MAX_INPUT_LENGTH 1024

int main()
{
    char input[MAX_INPUT_LENGTH];

    // read cmd line input from std input
    fgets(input, sizeof(input), stdin);
    printf("recv info: %s\n", input);

    return 0;
}

