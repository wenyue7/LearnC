// 线程分离
// 一个新创建的线程是可结合(joinable )且可分离(detached)的。
// 当线程属于可结合时，它需要被其他线程join或者cancel回收资源,同时可以获取该线程的返回信息。
// 相反一个已经处于分离的线程是不能被join或cancel的，资源会在终止时自动释放,并且无法获取该进程的返回信息。
// 
//
// 其实在上面的例子中，已经有过通过join将一个线程结合，但是当在一个线程中通过调用pthread_join()来回收资源时，调用者就会被阻塞，如果需要回收的
// 线程数目过多时，效率就大大下降。比如在一个Web 服务器中， 主线程为每一个请求创建一个线程去响应动作，我们并不希望主线程也为了回收资源而被阻塞，
// 因为可能在阻塞的同时有新的请求，我们可以再使用下面的方法，让线程办完事情后自动回收资源。
// 1 ). 在子线程中调用pthread_detach( pthread_self() )。 
// 2 ). 在主线程中调用pthread_detach( tid )。
// 3 ). 可以使用 pthread_attr_setdetachstate 函数对 pthread_attr_t 对象设置脱离属性。第一个参数是一个指向线程属性对象的指针,第二个参数
//      是脱离状态。因为可等待状态(joinable)是默认的,只有创建脱离线程的时候才需要调用这个函数;传递 PTHREAD__CREATE__DETACHED 作为第二个参数。
//      例如:
//      pthread_attr_t attr;
//      pthread_t thread;
//      pthread_attr_init (&attr);
//      pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
//      pthread_create (&thread, &attr, &thread_function, NULL);
//      pthread_attr_destroy (&attr);
// 
//
// 创建线程之后可以join，也可以detach。
// 如果 detach 了，再进行 join ，会 join 失败
// 如果 join 了，在进行 detach，会卡死在 pthread_join，因为一直等不到返回，即也会失败
//
// 无论是否 join 或者 detach，主线程退出都会导致子线程死亡

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>     // sleep()

void* run(void * arg)
{
    int loop = 0;

    pthread_detach(pthread_self());
    printf("I will detach ... \n");

    for (loop = 0; loop < 3; loop++) {
        printf("thread loop:%d\n", loop);
        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, run, NULL);

    sleep(1); // 因为主线程不会挂起等待，为了保证子线程先执行完分离，让主线程先等待1s

    if (pthread_join(tid1, NULL))
        printf(" join failed. \n");  // 在子线程中做了detach，所以无法再进行join。
    else
        printf(" join sucess. \n");

    return 0;
}
