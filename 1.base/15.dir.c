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
#include <sys/stat.h>
#include <sys/types.h>

int directory_exists(const char *path)
{
    struct stat status;
    if (stat(path, &status) == 0) {
        if (S_ISDIR(status.st_mode)) {
            return 1; // 目录存在
        }
    }
    return 0; // 目录不存在或发生错误
}

void rmdir_recursive(const char *path, int remove_top_dir)
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

    const char *path = "my_directory";
    mode_t mode = 0755; // 权限设置，类似于chmod命令中的权限设置

    if (directory_exists(path)) {
        printf("Directory exists, to delete\n");
        rmdir_recursive(path, 1);
    } else {
        printf("Directory does not exist, to create\n");
        // 调用mkdir函数创建文件夹
        if (mkdir(path, mode) == -1) {
            perror("Error making directory");
            return 1;
        }

    }
    return 0;
}
