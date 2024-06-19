/*************************************************************************
    > File Name: test.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed 19 Jun 2024 11:23:40 AM CST
 ************************************************************************/

#include <stdio.h>
#include "test.h"

int m_dump(Data *in_data)
{
    int result = 0;

#if 0
    result = (in_data->val1 + in_data->val2) * 3;
#else
    // 使用asm块嵌入汇编代码
    asm volatile(
        // 输入加载方式1: 通过堆栈直接读
        // "mov -0x18(%%rbp),%%rax\n"
        // "mov (%%rax),%%edx\n"
        // "mov -0x18(%%rbp),%%rax\n"
        // "mov 0x4(%%rax),%%eax\n"

        // 输入加载方式2: 通过 "D"(in_data) 指定的 rdi 寄存器
        "movl (%%rdi), %%eax\n"  // 将in_data->val1加载到eax
        "movl 4(%%rdi), %%edx\n"  // 将in_data->val2加载到edx

        // 直接copy gcc -S 的汇编代码或者 elf 文件反汇编的代码
        "add %%eax, %%edx\n"
        "mov %%edx, %%eax\n"
        "add %%eax, %%eax\n"
        "add %%edx, %%eax\n"
        "mov %%eax,-0x4(%%rbp)\n"

        // 输出计算结果
        "mov %%eax, %%esi\n"  // 将结果移动到result变量

        // 这是输出约束，它告诉编译器result变量将是汇编代码的输出，并且结果应该
        // 存储在%esi寄存器中。"=S"中的=表示这是一个输出变量，S表示%esi寄存器。
        // 在x86-64架构中，%esi是第二个整数参数寄存器，用于传递函数的第二个整数
        // 类型参数或返回值。
        : "=S"(result)  // 输出约束，将结果存储在result中
        // 这是输入约束，它告诉编译器in_data变量将是汇编代码的输入，并且它的值
        // 应该通过%rdi寄存器传递给汇编代码。"D"表示%rdi寄存器。在x86-64架构中，
        // %rdi是第一个参数寄存器，用于传递函数的第一个参数。
        : "D"(in_data)  // 输入约束，in_data作为输入参数
        // 这是clobber列表，它告诉编译器在执行这个asm块时，%rax和%rdx寄存器的值
        // 将会被修改。因此，编译器不会假设这些寄存器在汇编代码执行前后保持不变。
        // 在x86-64架构中，%rax通常用于存储返回值，而%rdx可以用作辅助寄存器。
        : "%rax", "%rdx"  // clobber列表，表示这些寄存器会被修改
    );

#endif
    printf("result is: %d\n", result);

    return 0;
}
