/* 
 * 参考博客:http://www.cnblogs.com/chenliyang/p/6633739.html
 * int getopt(int argc, char * const argv[], const char *optstring);
 * argc：main()函数传递过来的参数的个数
 * argv：main()函数传递过来的参数的字符串指针数组
 * optstring：选项字符串，告知 getopt()可以处理哪个选项以及哪个选项需要参数
 * 如果选项成功找到，返回选项字母；如果所有命令行选项都解析完毕，返回 -1；如果遇到选项字符不在 optstring 中，
 * 返回字符 '?'；如果遇到丢失参数，那么返回值依赖于 optstring 中第一个字符，如果第一个字符是 ':' 则返回':'，
 * 否则返回'?'并提示出错误信息。
 * 注意：在C语言中，单引号括起来的字符实际上代表一个整数，因此可以直接用 getopt 返回的 int 类型数据和单引号括起来的字母比较，
 *       例如代码示例中直接使用的 switch 语句
 *
 * 举例说明optstring的格式意义：
 * char *optstring = “ab:c::”;
 * 单个字符a         表示选项a没有参数           格式：-a即可，不加参数
 * 单字符加冒号b:     表示选项b有且必须加参数      格式：-b 100或-b100,但-b=100错
 * 单字符加2冒号c::   表示选项c可以有，也可以无    格式：-c200，其它格式错误
 * 上面这个 optstring 在传入之后，getopt 函数将依次检查命令行是否指定了 -a， -b， -c(这需要多次调用 getopt 函数，
 * 直到其返回-1)，当检查到上面某一个参数被指定时，函数会返回被指定的参数名称(即该字母)
 * optarg —— 指向当前选项参数(如果有)的指针。
 * optind —— 再次调用 getopt() 时的下一个 argv指针的索引。
 * optopt —— 最后一个未知选项。
 * opterr ­—— 如果不希望getopt()打印出错信息，则只要将全域变量opterr设为0即可。
 * 
 * 注意: getopt是用来解析命令行选项参数的，但是只能解析短选项: -d 100,不能解析长选项：--prefix
 */

// usage: ./getopt -a10 -b 20 -c 30 -d

#include<stdio.h>
#include<unistd.h>
#include<getopt.h>

int main(int argc, char **argv)
{
    int opt;
    char *string = "a::b:c:d";
    while ((opt = getopt(argc, argv, string))!= -1)
    { 
        printf("opt = %c\t\t", opt);
        printf("optarg = %s\t\t",optarg);
        printf("optind = %d\t\t",optind);
        printf("argv[optind] = %s\n",argv[optind]);
        switch(opt){
        case 'a':
            printf("----------- a\n");
            break;
        case 'b':
            printf("----------- b\n");
            break;
        default:
            break;
        }
    }  
}
