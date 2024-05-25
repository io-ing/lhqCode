#include <stdio.h>
#include <stdlib.h>

void func()
{
    static int staticVar = 10;
    printf("staticVar == %d\n", staticVar);
    staticVar++;
}

int main()
{
    func();
    func();
    func();
    
    exit(0);
}
