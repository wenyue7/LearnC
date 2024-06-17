/*************************************************************************
    > File Name: conv_str_hex.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed 10 Apr 2024 11:54:06 AM CST
 ************************************************************************/

// usage:
//     1. echo "0123456789abcdef0123456789abcdef" | ./conv_str_hex
//     2. ./conv_str_hex_demo 0123456789abcdef0123456789abcdef

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_EN 1

#define DUMP_INFO(...) \
    do { \
        if (DEBUG_EN) { \
            printf(__VA_ARGS__); \
        } \
    } while (0)

#define BYTE_IN_STR(str) ((int)(strlen(str) + 1) / 2)
#define DUMP_BUF(buf, cnt) \
    do { \
        int i; \
        if (DEBUG_EN) { \
            for (i = 0; i < cnt; i++) { \
                printf("0x%02x ", buf[i] & 0xFF); \
            } \
        } \
    } while (0)

static int flip_string(char *str)
{
    int len = strlen(str);
    int i, j;

    for (i = 0, j = len - 1; i <= j; i++, j--) {
        // swapping characters
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
    }

    return 0;
}

static int convert_str_to_hex(char *instr, char *outbuf)
{
    int i, loop_cnt;
    char str[64] = {0};
    char *endptr, c_tmp;

    loop_cnt = strlen(instr);
    DUMP_INFO("convert str to hex\n");
    memcpy(str, instr, strlen(instr));
    flip_string(str);
    loop_cnt = strlen(str);

    DUMP_INFO("in  str: %s\n", instr);
    DUMP_INFO("out buf: ");
    for (i = 0; i < loop_cnt; i++) {
        c_tmp = str[i];
        if (i % 2 == 0)
            outbuf[i / 2] = (strtol(&c_tmp, &endptr, 16) & 0xF);
        else
            outbuf[i / 2] |= (strtol(&c_tmp, &endptr, 16) & 0xF) << 4;
    }
    DUMP_BUF(outbuf, BYTE_IN_STR(str));
    DUMP_INFO("\n\n");

    return 0;
}

static int convert_hex_to_str(char *inbuf, char *outstr, int cnt)
{
    int i;

    DUMP_INFO("convert hex to str\n");
    DUMP_INFO("in  buf: ");
    DUMP_BUF(inbuf, cnt);
    DUMP_INFO("\n");

    for (i = 0; i < cnt; i++) {
        sprintf(outstr + i * 2, "%1x", inbuf[i] & 0xF);
        sprintf(outstr + i * 2 + 1, "%1x", (inbuf[i] >> 4) & 0xF);
    }
    flip_string(outstr);
    DUMP_INFO("out str: %s\n\n", outstr);

    return 0;
}

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    char instr[64];
    char buf[64];
    char outstr[64];

    if (argv[1]) {
        memcpy(instr, argv[1], strlen(argv[1]));
    } else {
        fgets(instr, sizeof(instr), stdin);
        instr[strcspn(instr, "\n")] = '\0';
        // memcpy(instr, "abc", strlen("abc") + 1);
    }

    convert_str_to_hex(instr, buf);
    convert_hex_to_str(buf, outstr, BYTE_IN_STR(instr));


    return 0;
}

