#include <stdio.h>
#include <stdlib.h>

#include "sqlist.h"

sqlist *sqlist_create()
{
    sqlist *sq = NULL;
    sq = malloc(sizeof(*sq));
    if (sq == NULL)
    {
        printf("malloc error\n");
        return NULL;
    }
    sq->last = -1;
    return sq;
}

int sqlist_create1(sqlist **me)
{

}

int sqlist_insert(sqlist *me, int pos, datatype *data)
{
    sqlist *sq = me;
    int i = 0;

    if (me == NULL)
        return -1;
    if (pos < 0 || pos > me->last)
        return -1;

    for (i = me->last; i>=pos; i--)
    {
        sq->data[i+1] = sq->data[i];
    }

// lzl make mistake pos to i
    sq->data[pos] = *data;
    sq->last++;
    return 0;
}

int sqlist_delete(sqlist *me, int pos)
{
}

int sqlist_find(sqlist *me, datatype *data)
{
}

int sqlist_isempty(sqlist *me)
{
}

int sqlist_setempty(sqlist *me)
{
}

int sqlist_getNum(sqlist *me)
{
}

int sqlist_destory(sqlist *me)
{
    free(me);
    me = NULL;
}

int sqlist_union(sqlist *list1, sqlist *list2)
{
}



int sqlist_display(sqlist *me)
{
    int i = 0;
    for (i=0; i<me->last; i++)
    {
        printf("%d\t", me->data[i]);
    }
    puts("\n");
}
