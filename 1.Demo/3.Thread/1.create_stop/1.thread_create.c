//使用 gcc或者g++ 编译需要增加 -lpthread 库。  GNU/Linux 实现了 POSIX 标准线程 API(所谓 pthreads)。所有线程函数和数据类型
//                                  都在 <pthread.h> 头文件中声明。这些线程相关的函数没有被包含在 C 标准库中,而是在
//                                  libpthread 中,所以当链接程序的时候需在命令行中加入 -lpthread 以确保能正确链接。
//
//
//一篇很不错的博客: https://blog.csdn.net/a1414345/article/details/70947396
//线程主要有三种状态分别是就绪、阻塞、运行。
//就绪：线程具备运行的所有条件，逻辑上已可以运行，在等待处理机。
//阻塞：指线程在等待某一事件的发生，如I/O操作。
//运行：占有处理器正在运行。
//
//
//int pthread_create(pthread_t *thread,
//                   const pthread_attr_t *attr,
//                   void *(*start_routine)(void *),
//                   void *arg);
//函数 pthread_create 负责创建新线程。你需要给它提供如下信息:
//    1、一个指向 pthread_t 类型变量的指针;新线程的线程 ID 将存储在这里。
//    2、一个指向 线程属性( thread_attr_t ) 对象的指针。这个对象控制着新线程与程序其它部分交互的具体细节。如果传递 NULL 作为
//       线程属性,新线程将被赋予一组默认线程属性。线程属性支持程序改变线程的各个方面，比如栈大小，调度参数及初始分离（detach）状态。
//    3、一个指向线程函数的指针。这是一个普通的函数指针,这个函数接收void指针作为参数，返回值也是void指针，类型如下(可以参考下边的示例):
//       void * (*) (void*);
//    4、一个线程参数,类型 void*。不论你传递什么值作为这个参数,当线程开始执行的时候,它都会被直接传递给新的线程。
//出错时返回错误码（不使用errno），参考下面的实例
//
//
//main函数所在的线程称为默认线程或主线程
//与fork()函数类似，新的线程会继承绝大多数属性、功能及父线程的状态。与fork()函数不同的是，线程会共享父进程的资源，而不是接收一份拷贝。
//当然，最终要的共享资源是进程地址空间。但是线程也共享（通过接收拷贝）信号处理函数和打开的文件。
//
//
//线程ID（TID）类似于进程ID（PID），但PID由linux内核分配，而TID由Pthread库分配。TID是由模糊类型pthread_t表示的，POSIX不要求他是
//个算术类型。因此比较的时候不能确保等好可以正常工作，因此Pthread库提供了接口用于比较TID：
//#includde <pthread.h>
//int pthread_equal(pthread_t t1, pthread_t t2);
//相同返回非零值，不同返回0。
//示例：
// int ret;
// ret = pthread_equal(thing1, thing2);
// if(ret != 0)
//     printf("The TIDs are equal!\n");
// else
//     printf("The TIDs are unequal!\n");

#include <stdio.h>
#include <errno.h>
#include <pthread.h>    // pthread_create()
#include <unistd.h>     // sleep()
#include <sys/types.h>  // getpid()

// 打印每个线程的ID， 和进行ID

void * run_1(void *arg)  // 线程1 执行代码
{
    sleep(1);
    printf(" thread 1 tid is  %u,  pid is %u \n", pthread_self(), getpid()); 
}

void * run_2(void *arg)  // 线程2 执行代码
{

    sleep(1);
    printf(" thread 2 tid is  %u,  pid is %u \n", pthread_self(), getpid());
}

int main()
{
    pthread_t tid1, tid2;
    int ret;

    // 创建线程1
    ret = pthread_create(&tid1, NULL, run_1, NULL );
    if(ret){
        errno = ret;
        perror("pthread_create");
        return -1;
    }

    // 创建线程2
    ret = pthread_create(&tid2, NULL, run_2, NULL );
    if(ret){
        errno = ret;
        perror("pthread_create");
        return -1;
    }

    printf("I am main tid is  %u,  pid is %u \n", pthread_self(), getpid());
    sleep(2);  //由于各个线程独立运行,但主线程结束之后子线程也均会结束,为了避免在子线程运行结束之前主线程结束,所以增加主线程延时
    return 0;
}
