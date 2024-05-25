#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *p = NULL;
    int count = 0;

    while(1)
    {
        p = fopen("temp", "r");
        if (p == NULL)
        {
            fprintf(stderr, "fopen() faild! errno = %d\n", errno);
            break;
        }
        count++;
    }

    printf("count = %d\n", count);

    exit(0);
}
