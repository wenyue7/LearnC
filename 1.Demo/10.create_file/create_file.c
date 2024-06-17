/*************************************************************************
    > File Name: create_file.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed Apr 10 10:53:44 2024
 ************************************************************************/

/*
 * 文件的类型和编码方式由写入的内容决定，而不是打开/创建方式决定，即便以二进制
 * 写入模式打开文件，但是使用fprintf写入字符串，也会变成文本文件
 *
 * 需要注意的是，写入二进制文件时一般用fwrite，而写入文本文件一般使用fprintf
 *
 * 如果写入中文，编码方式会默认为Unicode text, UTF-8 text
 * 如果写入英文，或者数字字符串，编码方式会默认为 ASCII text
 */

#include <stdio.h>

int create_txt(char *file_name)
{
    FILE *file_ptr;

    // 使用写入模式打开文件，如果文件不存在则创建，如果文件存在则清空内容
    file_ptr = fopen(file_name, "w");

    if (file_ptr == NULL) {
        printf("create file fail!\n");
        return 1;
    }

    // write data to file
    fprintf(file_ptr, "这是txt文件的内容\n");

    fclose(file_ptr);

    return 0;
}

int create_ascii(char *file_name)
{
    FILE *file_ptr;

    // 使用写入模式打开文件，如果文件不存在则创建，如果文件存在则清空内容
    file_ptr = fopen(file_name, "w");

    if (file_ptr == NULL) {
        printf("create file fail!\n");
        return 1;
    }

    // write data to file
    fprintf(file_ptr, "This context of ascii file\n");

    fclose(file_ptr);

    return 0;
}

int create_bin(char *file_name)
{
    FILE *file_ptr;

    // 使用二进制写入模式打开文件，如果文件不存在则创建，如果文件存在则清空内容
    file_ptr = fopen(file_name, "wb");

    if (file_ptr == NULL) {
        printf("create file fail!\n");
        return 1;
    }

    // write data to file
    char data[] = {0x01, 0x02, 0x03, 0x04};
    fwrite(data, sizeof(char), sizeof(data), file_ptr);

    fclose(file_ptr);

    return 0;
}


int create_cvs(char *file_name)
{
    FILE *file_ptr;

    // 使用写入模式打开文件，如果文件不存在则创建，如果文件存在则追加内容
    file_ptr = fopen(file_name, "a");

    if (file_ptr == NULL) {
        printf("create file fail!\n");
        return 1;
    }

    // writ csv data to file
    fprintf(file_ptr, "Name,Age,Grade\n");
    fprintf(file_ptr, "John,25,A\n");
    fprintf(file_ptr, "Emily,30,B\n");

    fclose(file_ptr);

    return 0;
}

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    create_txt("file_text.txt");
    create_ascii("file_ascii.txt");
    create_bin("file_binary.bin");
    create_cvs("file_csv.csv");
}
