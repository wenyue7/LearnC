//linux进程和线程，以及fork、wait、waitpid函数简单介绍  https://blog.csdn.net/LYhani82475/article/details/79867456

//创建进程使用fork()函数，执行进程使用exec函数族，终止进程则使用exit()和_exit()函数。exit()是先调用终止处理程序,清除I/O缓冲之后再调用内核的退出程序,而_exit()是直接调用内核的退出程序

//exit 和 return 的区别:调用return会释放局部变量,并弹栈,回到上级函数并执行,exit是直接退出,在使用vfork时如果调用了return可能会产生错误,因为return对共享的栈进行了操作,可能还对其他的资源进行了操作,这里不细究

//进程的三个基本状态：就绪、阻塞、执行

//fork函数创建子进程，无参数
//返回值为-1：创建进程失败
//返回值为 0：子进程
//返回值为大于0：父进程 子进程共享父进程的资源（代码） 一次调用两次返回：创建子进程给子进程返回0（成功）或-1（失败），给父进程返回子进程的ID号. 因为任何进程的 ID 均不为 0,程序可以藉此很轻松的判断自身运行在哪个进程中。

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t pid;
   
    printf("Process Creation Study\n");
    pid = fork();
    switch(pid)
    {
       case 0:
         printf("Child process is running,ChildPid is %d,ParentPid is %d\n",
                            getpid(),getppid());
         break;
       case -1:
         printf("Process creation failed\n");
         break;
       default:
         printf("Parent process is running,ChildPid is %d,ParentPid is %d\n",
                             pid,getpid());
        break;
    }
    exit(0); 
    return 0;
}
