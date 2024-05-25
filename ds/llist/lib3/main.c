#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llist.h"

#define NAMESIZE 64

struct score_st
{
    int id;
    char name[NAMESIZE];
    int math;
    int chinese;
};

static void print_s(const void *record)
{
    const struct score_st *r = record;
    printf("%d %s %d %d\n", r->id, r->name, r->math, r->chinese);
}

static int id_cmp(const void *key, const void *record)
{
    const int *k = key;
    const struct score_st *r = record;
    
    return (*k - r->id);
}

static int name_cmp(const void *key, const void *record)
{
    const char *k = key;
    const struct score_st *r = record;

    return strcmp(k, r->name);
}

int main()
{
    LLIST *handler;
    int i, ret;

    // create llist
    struct score_st tmp;
    handler = llist_create(sizeof(tmp));
    if (handler == NULL)
        exit(1);

    // insert llist
    for (i = 0; i < 7; i++)
    {
        tmp.id = i;
        snprintf(tmp.name, NAMESIZE, "stu%d", i);
        tmp.math = rand() % 100;
        tmp.chinese = rand() % 100;

        ret = handler->insert(handler, &tmp, LLIST_BACKWARD);
        if (ret)
            exit(1);
    }

    // travel llist
    handler->travel(handler, print_s);

    printf("\n");
    
    // find llist
    int id = 3;
    struct score_st *data;
    
    data = handler->find(handler, &id, id_cmp);
    if (data == NULL)
        printf("Can not find!\n");
    else
        print_s(data);

    printf("\n");

    //delete llist
    char *del_name = "stu6";
    ret = handler->delete(handler, del_name, name_cmp);
    if (ret)
        printf("llist_delete failed!\n");

    // travel llist
    handler->travel(handler, print_s);

    // destory llist
    llist_destory(handler);
    exit(0);
}
