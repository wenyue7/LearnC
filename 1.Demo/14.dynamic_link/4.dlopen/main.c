/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 21 May 2024 09:12:00 AM CST
 ************************************************************************/

#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
    // mprintf(3);

    // 打开共享库
    void *handle = dlopen("libmlib.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    // 清除之前的错误
    dlerror();

    // 获取函数地址
    typedef int (*m_lib_func_t)(int);
    m_lib_func_t m_fun = (m_lib_func_t)dlsym(handle, "mprintf");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        fprintf(stderr, "%s\n", dlsym_error);
        dlclose(handle);
        return 1;
    }

    // 调用函数
    m_fun(3);

    // 关闭共享库
    dlclose(handle);
    return 0;
}

