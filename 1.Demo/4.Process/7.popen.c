/*************************************************************************
    > File Name: 2.3.popen.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed Apr 10 15:38:12 2024
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main ()
{
	FILE* stream = popen ("sort", "w");
	fprintf (stream, "This is a test.\n");
	fprintf (stream, "Hello, world.\n");
	fprintf (stream, "My dog has fleas.\n");
	fprintf (stream, "This program is great.\n");
	fprintf (stream, "One fish, two fish.\n");
	return pclose (stream);
}
