/*************************************************************************
    > File Name: mlib.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 21 May 2024 09:09:39 AM CST
 ************************************************************************/

#include <stdio.h>

int glb_num = 0;

int mprintf(int num)
{
    glb_num = 9;
    printf("======> func:%s line:%d num %d from outside\n", __func__, __LINE__, num);
    // printf("======> func:%s line:%d num %d from outside %d\n", __func__, __LINE__,
    //         num, glb_num);

    return 0;
}
