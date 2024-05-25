#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFSIZE 1024

int main(int argc, char *argv[])
{
    int num = 1111;
    char *ch = "2222";
    char buf[1024];


    snprintf(buf, 1024, "%d", num);
    printf("%d\n", atoi(ch));
    printf("%s\n", buf);
    exit(0);
}
