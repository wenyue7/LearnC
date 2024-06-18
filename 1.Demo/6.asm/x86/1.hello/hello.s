.section .data
hello:      .asciz "Hello, World!\n"  # 定义一个字符串
len = .-hello

.section .text
.globl _start  # 声明入口函数

# _start函数，程序入口
_start:
    # 调用print_hello函数
    call print_hello

    # 退出程序
    movl $1, %eax        # 系统调用号 (sys_exit)
    xorl %ebx, %ebx      # 返回值 (0)
    int $0x80            # 调用内核

# print_hello函数，打印"Hello, World!"
print_hello:
    # 使用write系统调用打印字符串
    movl $4, %eax        # 系统调用号 (sys_write)
    movl $1, %ebx        # 文件描述符 (stdout)
    movl $hello, %ecx    # 缓冲区地址
    movl $len, %edx      # 缓冲区大小
    int $0x80            # 调用内核

    ret                  # 返回调用点

.section .bss

