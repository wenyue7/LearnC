/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 18 Jun 2024 05:30:40 PM CST
 ************************************************************************/

#include <stdio.h>

// 使用嵌入汇编实现的加法函数
int add(int a, int b)
{
    int result;
    // 内联汇编部分的最后一行通过使用占位符将C变量传递给汇编代码，并明确指定哪些
    // 寄存器可能被修改，从而保证了内联汇编和C代码之间的正确交互。这样可以使编译器
    // 更好地优化代码，同时避免潜在的寄存器冲突和数据损坏。
    asm (
        "ADD %[res], %[val1], %[val2]" // 执行加法指令
        : [res] "=r" (result)          // 输出操作数
        : [val1] "r" (a), [val2] "r" (b) // 输入操作数
    );
    return result;
}

int main()
{
    int x = 5;
    int y = 7;
    int result = add(x, y);

    printf("The result of %d + %d is %d\n", x, y, result);

    return 0;
}

