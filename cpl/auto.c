#include <stdio.h>
#include <stdlib.h>

int main()
{
    auto int i;
    static int j;
    printf("i == %d\n", i);
    printf("j == %d\n", j);

    exit(0);
}
