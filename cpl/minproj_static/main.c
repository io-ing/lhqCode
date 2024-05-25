#include <stdio.h>

// 全局变量，使用 static 修饰
static int globalVariable = 10;

// 外部函数声明
extern void tryToAccessGlobalVariable();

int main() {
    printf("Initial globalVariable value: %d\n", globalVariable);

    // 调用外部函数尝试修改全局变量的值
    tryToAccessGlobalVariable();

    // 打印修改后的值
    printf("Modified globalVariable value: %d\n", globalVariable);

    return 0;
}

