#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Sleep for a specified number of seconds
// Usage: sleep <seconds>
// if no argument is provided, error out
// if the argument is not a valid positive integer, error out

int 
isdigit(int c) {
    return (c >= '0' && c <= '9');
}

int
main(int argc, char *argv[])
{
   // argc: 参数个数; argv: 包含参数的数组
    if (argc < 2) { // 检查命令行参数的数量是否小于 2
        fprintf(2, "Error: Missing argument. Usage: sleep <seconds>\n");
        exit(1);
    }

    // 检查第一个参数是否为合法数字
    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (!isdigit(argv[1][i])) {  // 如果不是数字
            fprintf(2, "Error: Invalid argument '%s'. Argument should be a number.\n", argv[1]);
            exit(1);
        }
    }

    // 将第一个参数转为整型，并调用 sleep
    int seconds = atoi(argv[1]);
    if (seconds <= 0) {  // 如果转换后的时间小于等于 0
        fprintf(2, "Error: Invalid time value '%d'. Time must be a positive integer.\n", seconds);
        exit(1);
    }

    sleep(seconds);  
    exit(0);
}
