#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int someFunction()
{
    /* 这个函数只会返回 0、1 或 2 */
    return rand() % 3;
}

int main()
{
    int f = someFunction();

    switch (f)
    {
        case 0:
            printf("Case 0\n");
            break;
        case 1:
            printf("Case 1\n");
            break;
        case 2:
            printf("Case 2\n");
            break;
        default:
            fprintf(stderr, "An unexpected value: %d\n", f);
            /*
             * someFunction() 返回值一定是 012，但是出现了第四种情况，说明出了大问题，多半是写越界
             * 在这里不调用 exit() 防止触发钩子函数和 I/O 清理
             * 或者使用信号，例如：abort(); 来生成一个 core dump 以便进一步调试
             */
            _exit(EXIT_FAILURE);
    }

    return 0;
}
