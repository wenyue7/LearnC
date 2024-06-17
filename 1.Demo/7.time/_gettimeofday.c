// int gettimeofday(struct timeval *tv,struct timezone *tz);
// strut timeval {
//     long tv_sec; /* 秒数 */
//     long tv_usec; /* 微秒数 */
// };


// gettimeofday将时间保存在结构tv之中.tz一般我们使用NULL来代替.


// compile: gcc time.c -lm



#include <sys/time.h>
#include <stdio.h>
#include <math.h>

void function()
{
    unsigned int i,j;
    double y;
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
            y=sin((double)i);
}
int main()
{
    struct timeval tmstart,tmend;
    long timeuse;


    gettimeofday(&tmstart,NULL);
    function();
    gettimeofday(&tmend,NULL);


    timeuse = 1000000*(tmend.tv_sec-tmstart.tv_sec) + (tmend.tv_usec-tmstart.tv_usec);
    timeuse /= 1000;


    printf("Used Time:%ld ms\n",timeuse);

    return 0;
}
