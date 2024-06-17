/*************************************************************************
    > File Name: 2.2.thread_cond_wait_time.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: 2024年03月12日 星期二 22时28分07秒
 ************************************************************************/

/*
 * pthread_cond_timedwait()函数有三个入口参数：
 * (1)pthread_cond_t __cond：条件变量（触发条件）
 * (2)pthread_mutex_t __mutex: 互斥锁
 * (3)struct timespec __abstime: 等待时间（其值为系统时间 + 等待时间)
 * 当在指定时间内有信号传过来时，pthread_cond_timedwait()返回0，否则返回一个非0数，
 * 等待时间最长是设定的值，如果信号提前到达了，就会立刻返回，不需要等到设定时间，
 * 如果信号一直没有达到会在达到设定时间后返回一个非零数
 *
 *
 * 在使用pthread_cond_timedwait()函数时，必须有三步：
 * 1：加互斥锁：pthread_mutex_lock(&__mutex)
 * 2：等待：pthread_cond_timedwait(&__cond, &__mutex, &__abstime) //解锁->等待->加锁
 * 3：解互斥锁：pthread_mutex_unlock(&__mutex)
 *
 *
 * 发送信号量时，也要有三步：
 * 1：加互斥锁：pthread_mutex_lock(&__mutex)
 * 2：发送：pthread_cond_signal(&__cond)
 * 3：解互斥锁：pthread_mutex_unlock(&__mutex)
 *
 */



#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>

#define SENDSIGTIME 3

pthread_cond_t g_cond;
pthread_mutex_t g_mutex;

void thread1(void *arg)
{

    (void)arg;
    int ret = 0;
    struct timeval now;
    struct timespec outtime;

    pthread_mutex_lock(&g_mutex);

    gettimeofday(&now, NULL);
    outtime.tv_sec = now.tv_sec + 5;
    outtime.tv_nsec = now.tv_usec * 1000;

    ret = pthread_cond_timedwait(&g_cond, &g_mutex, &outtime);
    //ret = pthread_cond_wait(&g_cond, &g_mutex);
    pthread_mutex_unlock(&g_mutex);

    printf("thread 1 ret: %d\n", ret);

}

int main(void)
{

    pthread_t id1;
    int ret;

    pthread_cond_init(&g_cond, NULL);
    pthread_mutex_init(&g_mutex, NULL);

    ret = pthread_create(&id1, NULL, (void *)thread1, (void *)1);
    if (0 != ret)
    {

        printf("thread 1 create failed!\n");
        return 1;
    }

    printf("wait signal, %ds!\n", SENDSIGTIME);
    sleep(SENDSIGTIME);
    printf("sending signal...\n");
    pthread_mutex_lock(&g_mutex);
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_mutex);


    pthread_join(id1, NULL);

    pthread_cond_destroy(&g_cond);
    pthread_mutex_destroy(&g_mutex);

    return 0;
}

