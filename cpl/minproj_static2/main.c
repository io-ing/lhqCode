#include <stdio.h>

// 声明一个静态函数
void staticFunction() {
//static void staticFunction() {
    printf("This is a static function.\n");
}

// 外部函数声明
extern void tryToCallStaticFunction();

int main() {
    // 调用静态函数
    staticFunction();

    // 调用外部函数，尝试调用静态函数
    tryToCallStaticFunction();

    return 0;
}
