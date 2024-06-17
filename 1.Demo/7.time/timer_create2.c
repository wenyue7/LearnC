/* 参考文章: https://zhuanlan.zhihu.com/p/205609198
 *
 * 最强大的定时器接口来自POSIX时钟系列，其创建、初始化以及删除一个定时器的行动被分为三个不同的函数：timer_create()(创建定时器)、timer_settime()(初始化定时器)以及 timer_delete()(销毁它)。
 * POSIX Timer 对 setitimer 进行了增强，克服了 setitimer 的诸多问题：
 *   1、一个进程同一时刻只能有一个 timer。假如应用需要同时维护多个 Interval 不同的计时器，必须自己写代码来维护。这非常不方便。使用 POSIX Timer，一个进程可以创建任意多个 Timer。
 *   2、setitmer 计时器时间到达时，只能使用信号方式通知使用 timer 的进程，而 POSIX timer 可以有多种通知方式，比如信号，或者启动线程。
 *   3、使用 setitimer 时，通知信号的类别不能改变：SIGALARM，SIGPROF 等，而这些都是传统信号，而不是实时信号，因此有 timer overrun 的问题；而 POSIX Timer 则可以使用实时信号。
 *   4、 setimer 的精度是 ms，POSIX Timer 是针对有实时要求的应用所设计的，接口支持 ns 级别的时钟精度。
 * 
 * //---- 创建一个定时器
 * int timer_create(clockid_t clock_id, struct sigevent *evp, timer_t *timerid)
 *
 * 进程可以通过调用 timer_create() 创建特定的定时器，定时器是每个进程自己的，不是在 fork 时继承的。参数：
 *  clock_id 说明定时器是基于哪个时钟的，*timerid 装载的是被创建的定时器的 ID。该函数创建了定时器，并将他的 ID 放入timerid指向的位置中。参数evp指定了定时器到期要产生的异步通知。
 *  如果evp为 NULL，那么定时器到期会产生默认的信号，对 CLOCK_REALTIMER来说，默认信号就是SIGALRM。如果要产生除默认信号之外的其它信号，程序必须将 evp->sigev_signo设置为期望的信号码。
 *  struct sigevent 结构中的成员 evp->sigev_notify说明了定时器到期时应该采取的行动。通常，这个成员的值为SIGEV_SIGNAL，这个值说明在定时器到期时，会产生一个信号。程序可以将成员 
 *  evp->sigev_notify设为SIGEV_NONE来防止定时器到期时产生信号。
 * 
 * clock_id取值为以下:
 * CLOCK_REALTIME :Systemwide realtime clock. //时间是系统保存的时间，即可以由 date 命令显示的时间，该时间可以重新设置。
 * CLOCK_MONOTONIC:Represents monotonic time. Cannot be set.
 * CLOCK_PROCESS_CPUTIME_ID :High resolution per-process timer.  //CLOCK_PROCESS_CPUTIME_ID 的含义与 setitimer 的 ITIMER_VIRTUAL 类似。
 * CLOCK_THREAD_CPUTIME_ID :Thread-specific timer.// CLOCK_THREAD_CPUTIME_ID 以线程为计时实体，当前进程中的某个线程真正地运行了一定时间才触发 Timer。
 * CLOCK_REALTIME_HR :High resolution version of CLOCK_REALTIME.
 * CLOCK_MONOTONIC_HR :High resolution version of CLOCK_MONOTONIC.
 *
 *
 * //---- 启动一个定时器
 * int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspect *ovalue);
 *   struct itimespec{
 *      struct timespec it_interval;
 *      struct timespec it_value;
 *  };
 *
 * timer_create()所创建的定时器并未启动。要将它关联到一个到期时间以及启动时钟周期，可以使用timer_settime()。
 * 如同settimer()，it_value用于指定当前的定时器到期时间。当定时器到期，it_value 的值会被更新成 it_interval 的值。如果it_interval的值为0，则定时器不是一个时间间隔定时器，
 * 一旦 it_value 到期就会回到未启动状态。timespec的结构提供了纳秒级分辨率：
 *  struct timespec{
 *      time_t tv_sec;
 *      long tv_nsec;
 *  };
 *
 *
 * 参数：
 * 如果flags的值为TIMER_ABSTIME，则value所指定的时间值会被解读成绝对值(此值的默认的解读方式为相对于当前的时间)。这个经修改的行为可避免取得当前时间、计算“该时间”与
 * “所期望的未来时间”的相对差额以及启动定时器期间造成竞争条件。 如果ovalue的值不是NULL，则之前的定时器到期时间会被存入其所提供的itimerspec。如果定时器之前处在未启
 * 动状态，则此结构的成员全都会被设定成0。
 *
 *
 * //---- 获得一个活动定时器的剩余时间
 * int timer_gettime(timer_t timerid,struct itimerspec *value);
 *
 *
 * //---- 取得一个定时器的超限运行次数
 * int timer_getoverrun(timer_t timerid);
 *
 *
 * 有可能一个定时器到期了，而同一定时器上一次到期时产生的信号还处于挂起状态。在这种情况下，其中的一个信号可能会丢失。这就是定时器超限。程序可以通过调用timer_getoverrun来确定
 * 一个特定的定时器出现这种超限的次数。定时器超限只能发生在同一个定时器产生的信号上。由多个定时器，甚至是那些使用相同的时钟和信号的定时器，所产生的信号都会排队而不会丢失。
 * 执行成功时，timer_getoverrun()会返回定时器初次到期与通知进程(例如通过信号)定时器已到期之间额外发生的定时器到期次数。
 *
 *
 * //---- 删除一个定时器
 * int timer_delete (timer_t timerid);
 * 一次成功的timer_delete()调用会销毁关联到timerid的定时器并且返回0。执行失败时，此调用会返回-1并将errno设定会 EINVAL，这个唯一的错误情况代表timerid不是一个有效的定时器。
 *
 * POSIX Timer 到期通知方式
 *  SIGEV_NONE 定时器到期时不产生通知。。
 *  SIGEV_SIGNAL 定时器到期时将给进程投递一个信号，sigev_signo 可以用来指定使用什么信号。
 *  SIGEV_THREAD 定时器到期时将启动新的线程进行需要的处理
 *  SIGEV_THREAD_ID（仅针对 Linux) 定时器到期时将向指定线程发送信号。
 *
 *
 */


#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <time.h>


 void  handle(union sigval v){
     time_t t;
     char p[32];
     time(&t);
     strftime(p, sizeof(p), "%T", localtime(&t));
     printf("%s thread %lu, val = %d, signal captured.\n", p, pthread_self(), v.sival_int);
     return;
 }

int main(int argc, char *argv[]){

    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;
    int ret;
    memset(&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = handle;
    evp.sigev_value.sival_int = 3;   //作为handle()的参数
    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if( ret){
        perror("timer_create");
    }

    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 3;
    ts.it_value.tv_nsec = 0;
    ret = timer_settime(timer, TIMER_ABSTIME, &ts, NULL);
    if( ret )
    {
        perror("timer_settime");
    }

    while(1);

}
