/*************************************************************************
    > File Name: preload_debug.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 02 Jul 2024 10:41:30 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

__attribute__((constructor))
void preload_init()
{
    printf("Custom preload library loaded!\n");
}

