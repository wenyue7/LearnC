# 这个程序的基本工作流如下：

1. `_start` 是程序的入口点，从这里开始执行。
2. `_start` 调用 print_hello 函数。
3. `print_hello` 函数使用 write 系统调用将 "Hello, World!" 字符串打印到标准输出。
4. `print_hello` 返回到 `_start`。
5. `_start` 使用 exit 系统调用退出程序。


# 解释每个部分

数据段 (.data)
* hello 是一个以空字符结尾的字符串，存储在数据段中。

代码段 (.text)
* `.text` 段包含程序代码。
* `_start` 是程序的入口点。`bl print_hello` 调用 `print_hello` 函数。
* `print_hello` 使用 write 系统调用 (sys_write) 打印字符串。

未初始化数据段 (.bss)
* .bss 段通常用于未初始化的数据，本示例中没有使用。

# 系统调用说明

* `mov r7, #1`: 将系统调用号1（sys_exit）放入r7寄存器。
* `mov r0, #0`: 将返回值0放入r0寄存器，作为sys_exit的参数。
* `svc 0`: 触发软件中断，调用内核执行sys_exit系统调用，终止当前进程。
* `mov r7, #4`: 将系统调用号4（sys_write）放入r7寄存器。
* `mov r0, #1`: 将文件描述符1放入r0寄存器，表示标准输出（stdout）。
* `ldr r1, =hello`: 将字符串地址加载到r1寄存器。
* `mov r2, #14`: 将字符串长度加载到r2寄存器。
* `svc 0`: 触发软件中断，调用内核执行sys_write系统调用，将数据写入文件描述符1（标准输出）。

系统调用号：
`1`: 终止调用进程。
`4`: 将数据写入文件描述符。
