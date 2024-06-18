/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 18 Jun 2024 04:53:39 PM CST
 ************************************************************************/

#include <stdio.h>



int add(int a, int b)
{
    int result;
    asm(
        "addl %%ebx, %%eax;"  // 汇编指令：将 ebx 加到 eax
        : "=a" (result)       // 输出：将 eax 的值存储到 result 变量中
        : "a" (a), "b" (b)    // 输入：将 a 存储到 eax，b 存储到 ebx
    );
    return result;
}


int main()
{
    int num1 = 5;
    int num2 = 3;
    int result = add(num1, num2);

    printf("Result of %d + %d = %d\n", num1, num2, result);

    return 0;
}

