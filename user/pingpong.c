#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 1) // 检查命令行参数的数量是否正确
    {
        fprintf(2, "Error: Invalid number of arguments. Usage: pingpong\n");
        exit(1);
    }

    int p[2]; // 创建一个管道，存储两个文件描述符
    pipe(p); // 创建管道 p[0] 为读取端，p[1] 为写入端

    if (fork() == 0) // 如果是子进程，fork 返回 0
    {
        close(p[0]); // 子进程关闭读取端，子进程需要写数据
        char temp = 'x'; // 要传递的字节
        if (write(p[1], &temp, 1)) // 写一个字节到管道
        {
            fprintf(0, "%d: received ping\n", getpid()); // 打印子进程的 PID 和 "received ping"
        }
        close(p[1]); // 子进程写完后关闭写入端
    }
    else // 如果是父进程
    {
        wait((int *)0); // 父进程等待子进程结束
        close(p[1]); // 父进程关闭写端，因为父进程只需要读数据
        char temp;
        if (read(p[0], &temp, 1)) // 从管道读取一个字节
        {
            fprintf(0, "%d: received pong\n", getpid()); // 打印父进程的 PID 和 "received pong"
        }
        close(p[0]); // 父进程读完后关闭读取端
    }

    exit(0); // 退出程序
}
