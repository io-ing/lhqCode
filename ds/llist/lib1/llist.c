#include <stdlib.h>
#include <string.h>

#include "llist.h"

LLIST *llist_create(int initsize)
{
    LLIST *new;
    new = malloc(sizeof(*new));
    if (new == NULL)
    {
        exit(1);
    }

// zhuyi LLIST not struct llist_node_st
    new ->size = initsize;
    new->head.data = NULL;
    new->head.prev = &new->head;
    new->head.next = &new->head;

    return new;
}

int llist_insert(LLIST *ptr, const void *data, int mode)
{
    struct llist_node_st *newnode;

    newnode = malloc(sizeof(*newnode));
    if (newnode == NULL)
        return -1;

    newnode->data = malloc(ptr->size);
    if (newnode->data == NULL)
        return -2;

    memcpy(newnode->data, data, ptr->size);
    if (mode == LLIST_FORWARD)
    {
        newnode->prev = &ptr->head;
        newnode->next = ptr->head.next;
    }
    else    if (mode == LLIST_BACKWARD)
            {
                newnode->next = &ptr->head;
                newnode->prev = ptr->head.prev;
            }
            else // error
            {
                return -3;
            }
    newnode->next->prev = newnode;
    newnode->prev->next = newnode;

    return 0;
}

// void *
static struct llist_node_st *find_(LLIST *ptr, const void *key, llist_cmp *cmp)
//static void *find_(LLIST *ptr, const void *key, llist_cmp *cmp)
{
    struct llist_node_st *cur;
    
    for(cur = ptr->head.next; cur != &ptr->head; cur = cur->next)
    {
        if (cmp(key, cur->data) == 0)
            break;
    }
    return cur;
}

void *llist_find(LLIST *ptr, const void *key, llist_cmp *cmp)
{
    return find_(ptr, key, cmp)->data;
}

int llist_delete(LLIST *ptr, const void *key, llist_cmp *cmp)
{
    struct llist_node_st *node;
    
    node = find_(ptr, key, cmp);
    if (node == &ptr->head)
        return -1;
    
    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node->data);
    free(node);

    return 0;
}

int llist_fetch(LLIST *ptr, const void *key, llist_cmp *cmp, void *data)
{
    struct llist_node_st *node;
    
    node = find_(ptr, key, cmp);
    if (node == &ptr->head)
        return -1;

    node->prev->next = node->next;
    node->next->prev = node->prev;
    
    if (data != NULL)
        memcpy(data, node->data, ptr->size);

    free(node->data);
    free(node);

    return 0;
}// 删除节点并返回

void llist_travel(LLIST *ptr, llist_op *op)
{
    struct llist_node_st *cur;
    
    for (cur = ptr->head.next; cur != &ptr->head; cur = cur->next)
        op(cur->data);
}// 遍历

void llist_destory(LLIST *ptr)
{
    // *next not next
    struct llist_node_st *cur, *next;

    for (cur = ptr->head.next; cur != &ptr->head; cur = next)
    {
        next = cur->next;
        free(cur->data);
        free(cur); 
    }
    free(ptr);
}
