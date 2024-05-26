#include <stdio.h>
#include <stdlib.h>

static void f2(void *p)
{
    printf("f2():%s\n", p);
}

static void f1(void *p)
{
    printf("f1():%s\n", p);
}

int main()
{
    int job1, job2, job3;

    /* Begin!End!..bbb...aaa..ccc...... */
    printf("Begin!");
    job1 = at_addjob(5, f1, "aaa");
    if (job1)
    {
        fprintf(stderr, "at_addjob():%s\n", strerror(-job1));
        exit(1);
    }
    job2 = at_addjob(2, f1, "bbb");
    if (job2)
    {
        fprintf(stderr, "at_addjob():%s\n", strerror(-job2));
        exit(1);
    }
    job3 = at_addjob(7, f1, "ccc");
    if (job3)
    {
        fprintf(stderr, "at_addjob():%s\n", strerror(-job3));
        exit(1);
    }
    printf("End!");

    while(1)
    {
        write(1, ".", 1);
        sleep(1);
    }

    exit(0);
}
