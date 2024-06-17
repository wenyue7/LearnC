//一个不错的博客 https://www.cnblogs.com/CodingUniversal/p/7396671.html

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int glob=10;

static void my_exit1(void)  //进程退出时调用函数
{
    printf("pid=%d first exit handler\n",getpid());
}

static void my_exit2(void)
{
    printf("pid=%d second exit handler\n",getpid());
}

int main()
{
    int local;
    pid_t pid;
    local=8;
    if(atexit(my_exit1)!=0){  //atexit函数是一个特殊的函数，它是在正常程序退出时调用的函数，我们把他叫为登记函数（函数原型：int atexit (void (*)(void))）
        perror("atexit");     //为进程注册的退出时调用函数也会被子进程共享，先注册的后调用
    }                         
	//在gcc中，可以使用attribute关键字，声明constructor和destructor  具体参见: https://blog.csdn.net/king_cpp_py/article/details/79435351
	//还有: https://www.jianshu.com/p/37f18b7d0cca

    if(atexit(my_exit2)!=0){
        perror("atexit");
    }

    if((pid=fork())==0){
        printf("child pid is %d\n",getpid());   //子进程执行某个任务完后尽量使用exit退出，不然，若父进程中创建的子进程位于循环中，可能会引起未知的行为
    }
    else if(pid>0){
        sleep(10);
        glob++;
        local--;
        printf("father pid is %d\n",getpid());       
    }
    else{
        perror("fork");
    }
    printf("pid=%d,glob=%d,localar=%d\n",getpid(),glob,local);//这段代码父子进程共享
    return 0;//也可以使用exit(0)
}
