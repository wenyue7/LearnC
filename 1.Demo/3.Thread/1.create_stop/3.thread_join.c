// 有时候需要让一个线程去执行一段代码，我们需要知道它是否帮我们完成了指定的要求，或者异常终止，
// 这时候我们就需要获取线程运行结果，线程退出可以通过返回值带出或者通过pthread_exit()参数带出，
// 拿到它的“遗言”。
// 
// 还有一个用处，在上面的例子中，我们都是在主线程中sleep()函数来防止新创建的线程还未结束，
// 整个进程就结束，而现在我们可以用线程等待函数来达到这个目的。

// #include <pthread.h>
// int pthread_join(pthread_t thread, void ** retval);
// 描述：调用该函数的线程将挂起等待，直到id为thread的线程终止。如果线程已经终止，该函数会立即返回。一旦线程终止，调用线程就会醒来。
//      Pthread中所有线程都是对等节点，任何一个线程都可以join对方。一个线程可以join多个线程（这往往用在主线程等待其他线程）但是
//      应该只有一个线程尝试join某个特殊线程，多个线程不应该尝试随便join其他线程。thread线程以不同的方法终止，通过pthread_join()
//      得到的终止状态是不同的，主要有如下几种：
//         thread线程通过return 返回， retval 所指向的单元里存放的是 thread线程函数的返回值。
//         thread线程是被的线程调用pthread_cancel() 异常终止掉，retval 所指向的单元存放的是常数PTHREAD_CANCELED.
//         如果thread线程调用pthread_exit() 终止的，retval 所指向的单元存放的是传给pthread_exit的参数。
//      函数调用出错时会返回非零错误码。
//
//
// 默认情况下线程是可以join的。但是，线程也可以detach（分离），使得线程不可join。可以join的线程在被join之前占有的系统资源不会被释放。
// 而detach的进程会在进程结束的时候自动释放资源。

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* run_1(void *arg)
{
    int loop = 0;
    for (loop = 0; loop < 3; loop++) {
        printf("I am thread 1 loop:%d\n", loop);
        sleep(1);
    }
    return (void*)1; 
}

void* run_2(void *arg)
{
    int loop = 0;
    for (loop = 0; loop < 3; loop++) {
        printf("I am thread 2 loop:%d\n", loop);
        sleep(1);
    }

    pthread_exit((void*)2);
}


int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, run_1, NULL);
    pthread_create(&tid2, NULL, run_2, NULL);

    void* retval_1;
    void* retval_2;
    pthread_join(tid1, &retval_1);
    pthread_join(tid2, &retval_2);

    printf("thread 1 retval is  %p \n", (int*)retval_1);
    printf("thread 2 retval is  %p \n", (int*)retval_2);

    return 0;
}
