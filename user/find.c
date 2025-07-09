#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 获取路径中最后一个斜杠后的文件名
char *pfilename(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ; // 找到最后一个斜杠后的第一个字符
    p++;
    return p;
}

// 查找指定路径下的文件
int find(char *path, char *filename)
{
    int fd;  
    char buf[512], *p;  
    struct stat st;  
    struct dirent de;  

    // 打开目录
    if ((fd = open(path, 0)) < 0) 
    {
        // 打开失败
        fprintf(2, "open fail %s\n", path);
        exit(1);
    }

    // 获取文件或目录的信息
    if (fstat(fd, &st) < 0)
    {
        // 获取文件信息失败
        fprintf(2, "fstat fail %s\n", path);  
        close(fd);
        exit(1);
    }

    switch (st.type)
    {
        case T_FILE:  // 如果是常规文件
            if (0 == strcmp(pfilename(path), filename))  // 比较文件名与指定的文件名
            {
                fprintf(1, "%s\n", path);  // 打印匹配的文件路径
            }
            break;

        case T_DIR:  // 如果是目录
            strcpy(buf, path);  // 将给定路径复制到 buf 中
            p = buf + strlen(buf);  // 将指针移动到 buf 的末尾
            *p++ = '/';  // 在路径后添加一个斜杠

            // 读取目录的内容
            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if (de.inum == 0)  // 如果读取的条目的 inum 字段为 0，表示无效条目
                    continue;

                // 如果条目的名称是当前目录（.）或上级目录（..），跳过
                if (0 == strcmp(".", de.name) || 0 == strcmp("..", de.name))
                    continue;

                // 拼接路径，将条目的名称复制到 buf 中
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;  // 在 buf 数组中的目录项名称的末尾添加空字符，确保字符串有效

                // 获取该条目的信息
                if (stat(buf, &st) < 0)
                {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }

                // 递归查找子目录
                find(buf, filename);
            }
            break;
    }

    close(fd);  // 关闭文件描述符
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "not enough arguments\n");
        exit(1);
    }

    // 调用 find 函数，开始查找
    find(argv[1], argv[2]);

    exit(0);
}