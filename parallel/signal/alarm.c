#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    alarm(1);

    while(1);

    exit(0);
}
