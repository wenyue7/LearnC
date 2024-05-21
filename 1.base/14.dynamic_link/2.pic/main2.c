/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 21 May 2024 09:12:00 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

#include "mlib.h"

extern int glb_num;

int main(int argc, char *argv[])
{
    glb_num = 8;
    printf("======> func:%s line:%d call so func in main %d\n", __func__, __LINE__, glb_num);
    while (1) {
        sleep(1);
        printf("======> func:%s line:%d call so func in main %d\n", __func__, __LINE__, glb_num);
    }
    return 0;
}
