/*
 * 函数"clock_gettime"是基于Linux C语言的时间函数,可以用于计算时间，有秒和纳秒两种精度。
 * 函数原型：
 * int clock_gettime(clockid_t clk_id, struct timespec *tp);
 * 其中，cld_id类型四种：
 *   a、CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,中间时刻如果系统时间被用户改成其他,则对应的时间相应改变。
 *   b、CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
 *   c、CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间
 *   d、CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间
 *
 *
 * 本文默认采用CLOCK_REALTIME，即可实现并行程序的准确计时。
 * 其中，timespec结构包括：
 *   struct timespec {
 *      time_t tv_sec; // 秒
 *      long tv_nsec; // 纳秒
 *  };
 *
 */



#include <stdio.h>
#include <time.h>
#include <math.h>

void function()
{
    unsigned int i,j;
    double y;
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
            y=sin((double)i);
}

int main(int argc, char *argv[])
{
    struct timespec time1 = {0, 0};
    struct timespec time2 = {0, 0};

    clock_gettime(CLOCK_REALTIME, &time1);
    function();
    clock_gettime(CLOCK_REALTIME, &time2);

    printf("time passed is: %ld ms\n", (time2.tv_sec - time1.tv_sec)*1000 + (time2.tv_nsec - time1.tv_nsec)/1000000);

    return 0;
}
