#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int i;

    for (i=0; i<10; i++)
    {
        write(1, "*", 1);
        sleep(1);
    }

    exit(0);
}
