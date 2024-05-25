#include <stdio.h>
#include <stdlib.h>

int i = 0;

void printStar()
{
    for (i=0; i<5; i++)
        printf("*");
    printf("\n");
}
int main()
{
    for (i=0; i<5; i++)
        printStar();

    exit(0);
}
