#include <stdio.h>
#include <stdlib.h>

#define FILEPATH "/tmp/out"

int main(int argc, char **argv)
{
    if (acct(FILEPATH) < 0)
    {
        perror("acct()");
        exit(1);
    }

    sleep(100);
    acct(NULL);

    exit(0);
}
