/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 21 May 2024 09:12:00 AM CST
 ************************************************************************/

#include <stdio.h>

#include "mlib.h"

int main(int argc, char *argv[])
{
    printf("======> func:%s line:%d call so func in main\n", __func__, __LINE__);
    mprintf(3);
    return 0;
}
