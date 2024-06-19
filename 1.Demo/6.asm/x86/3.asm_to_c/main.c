/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed 19 Jun 2024 11:22:43 AM CST
 ************************************************************************/

#include <stdio.h>

#include "test.h"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    Data m_val = {3, 6};

    printf("hello wolrd!\n");
    m_dump(&m_val);
}
