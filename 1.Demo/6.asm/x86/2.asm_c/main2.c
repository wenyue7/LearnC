/*************************************************************************
    > File Name: main2.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 18 Jun 2024 06:04:46 PM CST
 ************************************************************************/

/*
 * x86 64 和 x86 32 的某些汇编指令不能通用，例如：64位系统使用movq，32位会使用movl
 * 这里的demo是64位系统使用的demo
 */

#include <stdio.h>

// 定义message字符串为全局变量
static const char message[] = "Hello, World!\n";

void print_hello_world()
{
    // 内联汇编部分
    // 内联汇编部分的最后一行通过使用占位符将C变量传递给汇编代码，并明确指定哪些
    // 寄存器可能被修改，从而保证了内联汇编和C代码之间的正确交互。这样可以使编译器
    // 更好地优化代码，同时避免潜在的寄存器冲突和数据损坏。
    __asm__ (
        "movq $1, %%rax;"           // syscall: write (1, stdout)
        "movq $1, %%rdi;"           // file descriptor: stdout
        "movq %[msg], %%rsi;"       // buffer: message
        "movq $14, %%rdx;"          // buffer size: 14 (length of "Hello, World!\n")
        "syscall;"                  // make the syscall
        :
        : [msg] "r"(message)        // input: message
        : "%rax", "%rdi", "%rsi", "%rdx" // clobbered registers
    );
}

int main()
{
    print_hello_world();
    return 0;
}

