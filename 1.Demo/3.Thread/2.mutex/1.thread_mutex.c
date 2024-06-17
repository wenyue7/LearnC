/*************************************************************************
    > File Name: 2.1.thread_mutex.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: 2024年03月12日 星期二 22时28分07秒
 ************************************************************************/

/*
 * 互斥锁
 *
 *
 * 初始化
 *
 * 静态创建
 * pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 该宏是一个结构体常量
 *
 * 动态创建
 * 通过pthread_mutex_init函数实现，函数原型如下：
 * int pthread_mutex_init(pthread_mutex_t*mutex, const pthread_mutexattr_t * attr);
 * 其中：
 *     mutex：所需创建的锁；
 *     attr：创建锁的属性。一般默认为NULL，分为以下几个属性：
 *       PTHREAD_MUTEX_TIMED_NP
 *         这是缺省值，也就是普通锁。当一个线程加锁以后，其余请求锁的线程将形成一个
 *         等待队列，并在解锁后按优先级获得锁。这种锁策略保证了资源分配的公平性；
 *       PTHREAD_MUTEX_RECURSIVE_NP
 *         嵌套锁，允许同一个线程对同一个锁成功获得多次，并通过多次unlock解锁。
 *         如果是不同线程请求，则在加锁线程解锁时重新竞争；
 *       PTHREAD_MUTEX_ERRORCHECK_NP
 *         检错锁，如果同一个线程请求同一个锁，则返回EDEADLK，否则与PTHREAD_MUTEX_TIMED_NP
 *         类型动作相同。这样就保证当不允许多次加锁时不会出现最简单情况下的死锁；
 *       PTHREAD_MUTEX_ADAPTIVE_NP
 *
 *
 * 加锁
 * 加锁即获取锁，通过pthread_mutex_lock()实现：
 * int pthread_mutex_lock(pthread_mutex_t *mutex);
 * 成功调用 pthread_mutex_lock() 会阻塞调用的线程，直到由mutex指向的互斥锁变得可用。
 * 一旦互斥锁可用了，调用线程就会被唤醒，函数返回0。如果在调用时互斥锁可用，函数会
 * 立即返回。出错时，函数可能返回的非0错误码。尝试获取已经持有的互斥锁会导致死锁。
 * 另外调用方往往不会检查返回值。直接执行： pthread_mutex_lock(&mutex);
 *
 *
 * 解锁
 * #include <pthread.h>
 * int pthread_mutex_unlock(pthread_mutex_t *mutex);
 * 成功调用pthread_mutex_unlock()会释放由mutex所指向的互斥锁，并返回0。该调用
 * 不会阻塞，互斥锁可以立即释放。出错时返回非零的错误码。
 * 调用方往往不会检查返回值。直接执行： pthread_mutex_unlock(&mutex);
 *
 *
 * 尝试加锁
 * int pthread_mutex_trylock(pthread_mutex_t *mutex);
 * pthread_mutex_trylock()语义与pthread_mutex_lock()类似，不同的是在锁已经被占据时
 * 返回EBUSY而不是挂起等待。
 *
 *
 * 注意，互斥锁只能由加锁的线程释放
 *
 */


#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;

// 多个线程调用该函数时实现互斥功能
void printVal(int num)
{
    pthread_mutex_lock(&m_mutex);
    printf("============== I am thread %d ==============\n", num);
    for(int i = 0; i < 10; i++){
        printf("tid is: %lu \n", pthread_self());
    }
    pthread_mutex_unlock(&m_mutex);
    return ;
}

void* run_1(void *arg)
{
    (void) arg;
    printVal(1);
    return (void*)1; 
}

void* run_2(void *arg)
{
    (void) arg;
    printVal(2);
    pthread_exit((void*)2);
}


int main()
{
    // initiate mutex
    pthread_mutex_init( &m_mutex, NULL ) ;

    pthread_t tid1, tid2;
    int ret;

    ret = pthread_create(&tid1, NULL, run_1, NULL);
    if(ret){
        errno = ret;
        perror("pthread_create");
        return -1;
    }
    ret = pthread_create(&tid2, NULL, run_2, NULL);
    if(ret){
        errno = ret;
        perror("pthread_create");
        return -1;
    }

    void* retval_1;
    void* retval_2;
    pthread_join(tid1, &retval_1);
    pthread_join(tid2, &retval_2);

    printf(" thread 1 retval is  %ls \n", (int*)retval_1);
    printf(" thread 2 retval is  %ls \n", (int*)retval_2);

    // destroy mutex
    pthread_mutex_destroy( &m_mutex ) ;

    return 0;
}
