#include <stdio.h>
#include <stdlib.h>

#define STRSIZE 32

int main()
{
    int int_var = 123;
    float float_var = 123.456;

    printf("int_var = %d\n",  int_var);
    printf("int_var = %8d\n", int_var);
    printf("int_var = %2d\n", int_var);

    printf("float_var = %f\n",    float_var);
    printf("float_var = %7f\n",   float_var);
    printf("float_var = %8.2f\n", float_var);
    printf("float_var = %3.2f\n", float_var);

    char str[STRSIZE] = "helloworld";

    printf("%s\n", str);
    printf("%.5s\n", str);
    printf("%-10.5s000\n", str);
    printf("%010.2f\n", float_var);

    printf("%d\n",  int_var);
    printf("%#o\n", int_var);
    printf("%#x\n", int_var);

    double double_var = 123.456;
    long long_var = 123l;
    long long llong_var = 123LL;
    float float_var2 = 123.456;

    printf("%lf\n", double_var);
    printf("%ld\n", long_var);
    printf("%lld\n", llong_var);

    printf("%ld %g\n", long_var, float_var2);
    printf("%ld %g\n", long_var);
    printf("%ld %g\n", long_var, float_var2, llong_var);

    //行缓冲模式：缓冲区满或者遇到换行\n进行刷新输出
    printf("[%s:%d] before while().", __FUNCTION__, __LINE__);
    printf("[%s:%d] before while().\n", __FUNCTION__, __LINE__);

    while(1);

    printf("[%s:%d] after while().\n", __FUNCTION__, __LINE__);

    printf("[%s:%d]\n", __FUNCTION__, __LINE__);
    sleep(5);
    printf("[%s:%d]", __FUNCTION__, __LINE__);

    exit(0);
}
