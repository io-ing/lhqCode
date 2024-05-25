#include <stdio.h>
#include <stdlib.h>

#include "sqlist.h"

int main()
{
    int ret = 0;
    sqlist *list = NULL;
    datatype data[] = {1, 6, 3, 4, 5};

    list = sqlist_create();
    if (list == NULL)
    {
        printf("create error\n");
        exit(1);
    }
    
    for (int i = 0; i<sizeof(data)/sizeof(*data); i++)
    {
        ret = sqlist_insert(list, 0, &data[i]);
        if (ret < 0)
            printf("insert error\n");
    }

    sqlist_display(list);


    exit(0);
}
