ANSI C 规范定义了 stof()、atoi()、atol()、strtod()、strtol()、strtoul() 共6个可以将字符串转换为数字的函数。
另外在 C99 / C++11 规范中又新增了5个函数，分别是 atoll()、strtof()、strtold()、strtoll()、strtoull()。

# strtol

`long int strtol (const char* str, char** endptr, int base);`
para：str 为要转换的字符串，endstr 为第一个不能转换的字符的指针，base 为字符串 str 所采用的进制。
ret：返回转换后的长整型数；
func：
  1. strtol() 会将参数 str 字符串根据参数 base 来转换成长整型数(long)。
  2. 参数 base 范围从2 至36，或0。
  3. 参数base 代表 str 采用的进制方式，如 base=10 则采用10 进制，若base=16 则采用16 进制等。
  4. strtol() 会扫描参数 str 字符串，跳过前面的空白字符（例如空格，tab缩进等，可以通过 isspace() 函数来检测），
     直到遇上数字或正负符号才开始做转换，再遇到非数字或字符串结束时(’\0’)结束转换，并将结果返回。
 
note:
  1. 当 base= 0 时，默认采用 10 进制转换，但如果遇到 '0x' / '0X' 前置字符则会
     使用 16 进制转换，遇到 '0' 前置字符则会使用 8 进制转换。
  2. 若endptr !=NULL，则会将遇到的不符合条件而终止的字符指针由 endptr 传回；
  3. 若 endptr = NULL，则表示该参数无效，或不使用该参数。
  4. 如果不能转换或者 str 为空字符串，那么返回 0(0L)；
  5. 如果转换得到的值超出 long int 所能表示的范围，函数将返回 LONG_MAX 或 LONG_MIN
     （在 limits.h 头文件中定义），并将 errno 的值设置为 ERANGE。

# atoi

`int atoi(const char *nptr);`
atoi()是C语言中的字符串转换成整型数的一个函数
func:atoi() 函数会扫描参数 str 字符串，跳过前面的空白字符（例如空格，tab缩进等），
     直到遇上数字或正负符号才开始做转换，而再遇到 非数字 或 字符串结束时(’\0’)
     才结束转换，并将结果返回。函数返回转换后的整型数；如果 str 不能转换成 int
     或者 str 为空字符串，那么将返回 0。

类似的函数还有：
```
long atol(const char *nptr);
long long atoll(const char *nptr);
