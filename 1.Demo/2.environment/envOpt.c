/*************************************************************************
    > File Name: envOpt.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Thu Sep 14 15:29:41 2023
 ************************************************************************/


// getenv()访问一个环境变量。输入参数是需要访问的变量名字，返回值是一个字符串。如果所访问的环境变量不存在，则会返回NULL。
// setenv()在程序里面设置某个环境变量的函数。
// unsetenv()清除某个特定的环境变量的函数。
// putenv() 改变或增加环境变量

#include <stdio.h>
#include <stdlib.h>

extern char**environ;

int main ()
{
	char *env_name = "env_test";
	char *envVal;
	char *new_env_val = "newVal";

	envVal = getenv(env_name);
	printf("old env val(%s):%s\n", env_name, envVal);
	// int setenv(const char *name,const char * value,int overwrite);
	// name为环境变量名称字符串。
	// value则为变量内容。
	// overwrite用来决定是否要改变已存在的环境变量。如果overwrite不为0，
	// 则改变环境变量原有内容，原有内容会被改为参数value所指的变量内容。
	// 如果overwrite为0，且该环境变量已有内容，则参数value会被忽略。
	setenv(env_name, new_env_val, 1);
	printf("set envVal to %s\n", new_env_val);

	envVal = getenv(env_name);
	printf("read new env val(%s):%s\n", env_name, envVal);

	printf("you can set env val %s: export %s=newVal\n", env_name, env_name);
	return 0;

}
