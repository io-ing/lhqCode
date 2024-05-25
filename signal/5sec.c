#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    time_t end;
    int64_t count = 0;

    end = time(NULL) + 5;

    while(time(NULL) <=  end)
        count++;

    printf("%ld\n", count);

    exit(0);
}
