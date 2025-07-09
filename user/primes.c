#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);  // 创建初始管道
    
    // 生成数字 2~35
    if (fork() == 0) {
        // 子进程：数字生成器
        close(fd[0]);  // 关闭读端
        for (int i = 2; i <= 35; i++) {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);  // 关闭写端
        exit(0);
    }

    // 主进程：质数筛
    close(fd[1]);  // 关闭写端
    
    while (1) {
        int prime;
        // 读取当前阶段的第一个数
        if (read(fd[0], &prime, sizeof(prime)) <= 0) {
            close(fd[0]);
            break;  // 没有更多数字，退出循环
        }
        printf("prime %d\n", prime); 
        
        int next_fd[2];
        pipe(next_fd);  // 为下一阶段创建新管道
        
        if (fork() == 0) {
            // 子进程：下一阶段的过滤器
            close(fd[0]);  // 关闭旧管道读端
            close(next_fd[1]);  // 关闭新管道写端
            
            // 将新管道设为当前管道，继续循环
            fd[0] = next_fd[0];
        } else {
            // 父进程：当前阶段的过滤器
            close(next_fd[0]);  // 关闭新管道读端
            
            int num;
            while (read(fd[0], &num, sizeof(num)) > 0) {
                if (num % prime != 0) {
                    write(next_fd[1], &num, sizeof(num));
                }
            }
            
            close(fd[0]);  // 关闭旧管道
            close(next_fd[1]);  // 关闭新管道写端
            wait(0);  // 等待子进程结束
            break;  // 当前阶段完成
        }
    }
    
    wait(0);  // 确保等待所有子进程
    exit(0);
}