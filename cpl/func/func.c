#include <stdio.h>
#include <stdlib.h>

#define M 3
#define N 4

//void print_arr2(int (*p)[N], int m)
void print_arr2(int p[][N], int m)
{
    int i = 0;
    int j = 0;

    for (i=0; i<m; i++)
    {
        for (j=0; j<N; j++)
        {
            printf("p[%d][%d] = %d\t", i, j, p[i][j]);
        }
        puts("\n");
    }
}

void print_arr(int *p, int n)
{
    int i = 0;

    for (i=0; i<n; i++)
    {
//        printf("p[%d] = %d\n", i, p[i]);
        printf("p[%d] = %d\n", i, *(p+i));
    }
}

int main()
{
    int a[M][N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int i = 0;
    int j = 0;

#if 0
    for (i=0; i<M; i++)
    {
        for (j=0; j<N; j++)
        {
            printf("a[%d][%d] = %d\t",i, j, a[i][j]);
        }
        puts("\n");
    }
#endif
    

//    print_arr(*a, sizeof(a)/sizeof(a[0][0]));
//    print_arr(*a, sizeof(a)/sizeof(*(*a)));

    print_arr2(a, M);

    exit(0);
}

