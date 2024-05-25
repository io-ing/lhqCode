#include <stdio.h>
#include <stdlib.h>

#define LEFT 3000000
#define RIGHT 3000200

int is_primer(int num)
{
    int i;

    for (i=2; i<num; ++i)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int i;

    for (i=LEFT; i<RIGHT; ++i)
    {
        if (is_primer(i))
        {
            printf("%d is primer\n", i);
        }
    }

    exit(0);
}
