/*************************************************************************
    > File Name: demo.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Mon 17 Jun 2024 04:41:58 PM CST
 ************************************************************************/

#include <stdio.h>

// 使用内联汇编来将两个整数相加
int add_with_asm(int a, int b) {
    int result;
    asm (
        "addl %2, %1\n\t" // 将%2（即b）加到%1（即result的内存地址指向的值）上
        "movl %1, %%eax\n\t" // 将结果移动到eax寄存器中（虽然这一步在GCC中可能是多余的）
        : "=r"(result) // 输出操作数，%1，将结果存放在result中
        : "0"(a), "r"(b) // 输入操作数，%0（即a，但这里我们不用它，所以用"0"占位），%2（即b）
        : // 没有破坏的寄存器列表（在这个例子中，我们假设没有）
    );
    return result;
}

int main() {
    int a = 5;
    int b = 10;
    int sum = add_with_asm(a, b);
    printf("The sum is: %d\n", sum);
    return 0;
}
