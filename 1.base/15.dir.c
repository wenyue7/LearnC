/*************************************************************************
    > File Name: 15.dir.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Fri May 24 17:51:12 2024
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

static int directory_exists(const char *path)
{
    struct stat status;
    if (stat(path, &status) == 0) {
        if (S_ISDIR(status.st_mode)) {
            return 1; // 目录存在
        }
    }
    return 0; // 目录不存在或发生错误
}

static int create_dir_recursive(const char *path, mode_t mode)
{
    char *p = NULL;
    char *dir_path = strdup(path); // 复制路径字符串，因为我们会修改它

    if (!dir_path) {
        perror("strdup");
        return -1;
    }

    for (p = dir_path + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0'; // 临时替换'/'为'\0'，以便我们可以创建当前部分的目录

            if (mkdir(dir_path, mode) == -1) {
                if (errno != EEXIST) { // 如果目录不存在且创建失败
                    perror("mkdir");
                    free(dir_path);
                    return -1;
                }
            }

            *p = '/'; // 恢复'/'
        }
    }

    // 创建最后一个目录（如果它不是根目录）
    if (strcmp(dir_path, "/") != 0 && mkdir(dir_path, mode) == -1) {
        if (errno != EEXIST) {
            perror("mkdir");
            free(dir_path);
            return -1;
        }
    }

    free(dir_path);
    return 0;
}

static void rmdir_recursive(const char *path, int remove_top_dir)
{
    DIR *dir;
    struct dirent *entry;
    char full_path[1024];

    if (!(dir = opendir(path))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // 递归删除子目录
            rmdir_recursive(full_path, 1);
        } else {
            // 删除文件
            unlink(full_path);
        }
    }
    closedir(dir);

    // 如果需要删除顶层目录，并且它是空的（现在应该是），则删除它
    if (remove_top_dir) {
        rmdir(path);
    }
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    const char *top_path = "my_directory";
    const char *create_path = "my_directory/dir1/dir2";
    mode_t mode = 0755; // 权限设置，类似于chmod命令中的权限设置

    if (directory_exists(top_path)) {
        printf("Directory exists, to delete\n");
        rmdir_recursive(top_path, 1);
    } else {
        printf("Directory does not exist, to create\n");
        // 调用mkdir函数创建文件夹
        // if (mkdir(top_path, mode) == -1) {
        //     perror("Error making directory");
        //     return 1;
        // }

        // 递归创建文件夹
        create_dir_recursive(create_path, mode);
    }
    return 0;
}
