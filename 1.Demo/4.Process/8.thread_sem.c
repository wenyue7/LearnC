//==========================================================================================================================
// 分类：
// 内核信号量
// 用户信号量--+--POSIX信号量--+--有名信号量
//             |               +--无名信号量
//             |
//             +--SYSTEMV信号量
//
// ---- 内核信号量 ----
// #include
// void sema_init(struct semaphore *sem, int val);
// void init_MUTEX(struct semaphore *sem); //初始值1
// void init_MUTEX_LOCKED(struct semaphore *sem); //初始值0
// void down(struct semaphore *sem); //可睡眠
// int down_interruptible(struct semaphore *sem); //可中断
// int down_trylock(struct semaphore *sem); //m非阻塞
// void up(struct semaphore *sem);
//
// ---- SYSTEMV信号量 ----
// #include
// int semget(key_t key, int nsems, int oflag);
// int semop(int semid, struct sembuf *opsptr, size_t nops);
// int semctl(int semid, int semum, int cmd,...);
//
// ---- POSIX无名信号量 ----
// #include
// sem_t sem;
// int sem_init(sem_t *sem, int pshared, unsigned int val); //pshared为0则线程间共享，pshared为1则父子进程共享
// int sem_wait(sem_t *sem); //阻塞
// int sem_trywait(sem_t *sem); //非阻塞
// int sem_post(sem_t *sem);
// int sem_destroy(sem_t *sem);
// 进程间共享则sem必须放在共享内存区域（mmap, shm_open, shmget），父进程的全局变量、堆、栈中存储是不行的
//
// ---- POSIX有名信号量 ----
// sem_t *sem_open(const char *name, int oflag, mode_t mode, int val);
// int sem_trywait(sem_t *sem);
// int sem_post(sem_t *sem);
// int sem_close(sem_t *sem);
// int sem_unlink(const char *name);
// 每个open的位置都要close和unlink，但只有最后执行的unlink生效
//
//==========================================================================================================================
//
// 这里重点描述了POSIX标准下的信号量使用。
// 信号量本质上是一个计数器，表征资源可用数量。大于0时资源可访问，小于等于0时，资源不可访问，线程只能等待；
//
// sem_t : 定义在semaphore.h头文件中
//
// sem_t相关的操作如下：
// int sem_init(sem_t* __sem, int __shared, unsigned int __value);
// int sem_wait(sem_t* __sem);
// int sem_post(sem_t* __sem);
// int sem_getvalue(sem_t* __sem, int* __value);
// int sem_timedwait(sem_t* __sem, const struct timespec* __ts);
// int sem_trywait(sem_t* __sem);
// int sem_destroy(sem_t* __sem);
//
//
// int sem_init(sem_t* __sem, int __shared, unsigned int __value);
// 初始化信号量，包含3个参数，分别是信号量指针，共享模式，value初始值，分别解释下：
// __sem: 要初始化的信号量指针
// __shared: 若为0，表示进程内私有，只能进程内多线程共享，若不为0，表示可以进程间共享（有说法不为0可能造成函数调用失败，属于无名信号量，待验证）
// __value: 信号量的初值
//
//
// int sem_wait(sem_t* __sem);
// 阻塞当前线程，直到信号量的值大于0，接触阻塞后将信号量的值减1，表示公共资源使用后减少；
// 是一个原子操作；
//
// int sem_post(sem_t* __sem);
// 该线程会释放资源，使信号量的值加1；当有某个线程等待这个信号量时，该线程即可以运行，并且将将信号量的值减1（参见sem_wait）；
//
//
// int sem_destroy(sem_t* __sem);
// 销毁信号量
// 


#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

sem_t sem;

void *testfunc2(void *arg)
{
    while(1)
    {
        sem_wait(&sem);
        //do something....
        printf("======> wait thd 2\n");
    }
}

void *testfunc1(void *arg)
{
    while(1)
    {
        sem_wait(&sem);
        //do something....
        printf("======> wait thd 1\n");
    }
}

int main()
{
    pthread_t pt1, pt2;
    sem_init(&sem, 0, 0);
    pthread_create(&pt1,NULL,testfunc1,NULL);
    pthread_create(&pt2,NULL,testfunc2,NULL);
    while(1)
    {
        // 每隔一秒执行一次 sem_post 信号量sem加1 子线程sem_wait解除等待
        // 执行一次sem_post sem 只能加1，也就意味着只能被减一次
        // 如果这里执行多次 sem_post ，那等待线程也就可以多次执行，即多次减一
        sem_post(&sem);
        printf("======> post\n");
        sleep(1);
    }

    sem_destroy(&sem);

    return 0;
}
