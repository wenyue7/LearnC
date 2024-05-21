/*************************************************************************
    > File Name: mlib.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 21 May 2024 09:09:39 AM CST
 ************************************************************************/

#include <stdio.h>

int mprintf(int num)
{
    printf("======> func:%s line:%d num %d from outside\n", __func__, __LINE__, num);

    return 0;
}
