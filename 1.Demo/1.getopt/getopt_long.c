/* 
 * int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts,int *longindex);
 * 描述：包含 getopt 功能，增加了解析长选项的功能如：--prefix --help
 * longopts    指明了长参数的名称和属性
 * longindex   如果longindex非空，它指向的变量将记录当前找到参数符合longopts里的第几个元素的描述，即是longopts的下标值
 * 对于短选项，返回值同getopt函数；对于长选项，如果flag是NULL，返回val，否则返回0；对于错误情况返回值同getopt函数
 *
 * struct option {
 * const char  *name;       // 参数名称,也是长选项的名称
 * int          has_arg;    // 指明是否带有参数,如果为 1 则表示该选项接受一个参数,否则为 0
 * int          *flag;      // flag=NULL时,函数返回val, 如下代码示例中： 可执行程序 --help, getopt_long 的返回值为 h;
 *                          // flag不为空时,可以设置一个变量，例如： {"http-proxy", required_argument, &lopt, 1 }, getopt_long返回0, lopt的值为1
 * int          val;        // 用于指定函数找到选项的返回值或flag非空时指定*flag的值
 * }; 
 *
 * has_arg  指明是否带参数值，其数值可选：
 * no_argument        表明长选项不带参数，如：--name, --help
 * required_argument  表明长选项必须带参数，如：--prefix /root或 --prefix=/root
 * optional_argument  表明长选项的参数是可选的，如：--help或 –prefix=/root，其它都是错误
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* 程序的名称。*/
const char* program_name;
/* 将程序使用方法输出到 STREAM 中(通常为 stdout 或 stderr),并以 EXIT_CODE 为返回值退出程序。函数调用不会返回。*/
void print_usage (FILE* stream, int exit_code)
{
	fprintf (stream, "Usage: %s options [ inputfile ... ]\n", program_name);
	fprintf (stream,
		" -h --help Display this usage information.\n"
		" -o --output filename Write output to file.\n"
		" -v --verbose Print verbose messages.\n");
	exit (exit_code);
}

/* 程序主入口点。ARGC 包含了参数列表中元素的数量;ARGV 是指向这些参数的指针数组。*/
int main (int argc, char* argv[])
{
	int next_option;
	/* 包含所有有效短选项字母的字符串。*/
	const char* const short_options = "ho:v";
	/* 描述了长选项的 struct option 数组。*/
	const struct option long_options[] = {
		{ "help", 0, NULL, 'h' },
		{ "output", 1, NULL, 'o' },
		{ "verbose", 0, NULL, 'v' },
		{ NULL, 0, NULL, 0 } /* 数组末要求这样一个元素。*/
	};
	/* 用于接受程序输出的文件名,如果为 NULL 则表示标准输出。*/
	const char* output_filename = NULL;
	/* 是否显示冗余信息?*/
	int verbose = 0;
	/* 记住程序的名字,可以用于输出的信息。名称保存在 argv[0]中。*/
	program_name = argv[0];
	do {
		next_option = getopt_long (argc, argv, short_options,
			long_options, NULL);
		switch (next_option){
			case 'h': /* -h 或 --help */
				/* 用户要求查看使用帮助。输出到标准输出,退出程序并返回 0(正常结束)。*/
				print_usage (stdout, 0);
			case 'o': /* -o 或 --output */
				/* 此函数接受一个参数,表示输出文件名。*/
				output_filename = optarg;
				(void) output_filename;
				break;
			case 'v': /* -v 或 --verbose */
				verbose = 1;
				break;
			case '?': /* The user specified an invalid option. */
				/* 向标准错误输出帮助信息,结束程序并返回 1(表示非正常退出)。*/
				print_usage (stderr, 1);
			case -1: /* 结束处理选项的过程。*/
				break;
			default: /* 别的什么:非预料中的。*/
				abort ();
		}
	}
	while (next_option != -1);
	/* 选项处理完毕。OPTIND 指向第一个非选项参数。
	 * 出于演示目的,如果指定了冗余输出选项,则输出这些参数。*/
	if (verbose) {
		int i;
		for (i = optind; i < argc; ++i)
			printf ("Argument: %s\n", argv[i]);
	}
	/* 主程序到这里结束。*/
	return 0;
}
