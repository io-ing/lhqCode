#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *p = NULL;

    p = fopen("temp", "r");
    if (p == NULL)
    {
#if 0
        fprintf(stderr, "fopen failed! errno = %d\n", errno);
#elif 0
        perror("fopen error!");
#else
        fprintf(stderr, "fopen failed! error = %s\n", strerror(errno));
#endif
    }

    puts("OK");
    exit(0);
}
