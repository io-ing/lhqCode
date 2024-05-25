// File: external.c
#include <stdio.h>

// 外部函数的实现
void tryToCallStaticFunction() {
    // 尝试调用 main.c 中的 staticFunction
    // 这里将会引发编译警告，但不会导致错误
    staticFunction();
    printf("Trying to call staticFunction from external.c\n");
}
