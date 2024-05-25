#include <stdio.h>
#include <stdlib.h>

int var = 0;            // 全局变量

int main()
{
    int var = 10;       // 局部变量
    {
        int var = 20;   // 局部变量
        printf("var == %d\n", var); // 20
    }
    exit(0);
}
