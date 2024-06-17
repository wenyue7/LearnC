// 如果需要只终止某个线程而不是整个进程都终止，有三种方法。
//     1). 从线程函数return，对主线程不使用，在main函数中return 相当于exit。
//     2). 线程可以调用 pthread_exit() 终止自己。这里也可以说是线程自杀。该函数不会出错。
//         #include <pthread.h>
//         void pthread_exit(void *retval);
//         retval：提供信息给需要等待结束线程的终止状态的线程。
//     3). 一个线程可以调用pthread_cancel() 终止同一进程中的另一个线程。
//         #include <pthread.h>
//         int pthread_cancel(pthread_t thread);
//         返回0表示调用成功，但是调用成功只是表示成功执行了取消请求，并不代表线程已经终止。实际的取消操作是异步的。
//         出错时会返回 ESRCH，表示 thread 是非法的。
//         线程如何取消以及何时取消有些复杂，这里不做深入分析。
//
//
// 有三种情况可以杀死进程中中所有线程：
//     进程从main()函数返回
//     进程从exit()函数终止
//     进程通过execve()函数执行新的二进制镜像

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


// 终止线程 方法1

void * run1(void * arg)
{
    printf("I am tread_1, I am still alive, after a second I will quit.\n");
    sleep(1);

    return NULL;

    printf("Never output.\n");
}

// 终止线程 方法2

void * run2(void *arg)
{
    sleep(1);
    printf("I am tread_2, I am still alive, I will quit.\n");
    pthread_exit(NULL);
    printf("never output .\n");
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, run1, NULL);
    pthread_create(&tid2, NULL, run2, NULL);

    sleep(2); // 确保主线程最后退出 
    printf("The thread quit, I should quit.\n");
    return 0;
}