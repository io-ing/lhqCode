#include <stdio.h>

// 外部函数实现
void tryToAccessGlobalVariable() {
    // 尝试访问并修改 main.c 中的 globalVariable
    // 这里将会引发编译错误
     globalVariable = 20;
    printf("Trying to modify globalVariable from external.c\n");
}

