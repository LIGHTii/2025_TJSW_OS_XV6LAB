#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char *args[MAXARG];  // 保存执行的参数
    int p = 0;
    int n = MAXARG;

    // 先把 xargs 自带的参数读进去
    for (int p_v = 0; p_v < argc; p_v++) {
        if (p_v==1 && strcmp(argv[p_v], "-n") == 0){
            n = atoi(argv[p_v + 1]);
            p_v++;
            continue;
        }
        args[p++] = argv[p_v];
    }

    char buf[256];

    // 进入循环，每次读一行内容
    while (1) {
        int i = 0;
        
        // 读取参数直至\n，或到达要求数量， 并将其对应存储至buf中
        while ((read(0, buf + i, sizeof(char)) != 0) && buf[i] != '\n') {
            i++;
            if(i % n == 0)
                break;
        }

        if (i == 0)  // 如果读取到的是空行，表示结束
            break;

        buf[i] = 0;  // 字符串结尾，exec 要求的格式
        args[p] = buf;  // 把标准输入传进的一行参数附加到 xargs
        args[p + 1] = 0;  // exec 读到 0 就表示参数结束

        // 子进程执行命令
        if (fork() == 0) {
            exec(args[1], args + 1);  // 执行命令，args[1] 是命令，args + 1 是命令参数

            // 如果 exec 执行失败，打印错误信息
            printf("exec error\n");
            exit(1);
        } else {
            wait((void *)0);  // 父进程等待子进程结束
        }
    }

    exit(0);
}
