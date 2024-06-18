.section .data
hello:      .asciz "Hello, World!\n"  // 定义一个字符串
len = .-hello

.section .text
.global _start  // 声明入口函数

// _start函数，程序入口
_start:
    // 调用print_hello函数
    bl print_hello

    // 退出程序
    mov r7, #1          // 系统调用号 (sys_exit)
    mov r0, #0          // 返回值 (0)
    svc 0               // 调用内核

// print_hello函数，打印"Hello, World!"
print_hello:
    // 使用write系统调用打印字符串
    mov r7, #4          // 系统调用号 (sys_write)
    mov r0, #1          // 文件描述符 (stdout)
    ldr r1, =hello      // 缓冲区地址
    ldr r2, =len        // 缓冲区大小
    svc 0               // 调用内核

    bx lr               // 返回调用点

.section .bss

