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
    // 内联汇编部分的最后一行通过使用占位符将C变量传递给汇编代码，并明确指定哪些
    // 寄存器可能被修改，从而保证了内联汇编和C代码之间的正确交互。这样可以使编译器
    // 更好地优化代码，同时避免潜在的寄存器冲突和数据损坏。
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

