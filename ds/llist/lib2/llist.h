#ifndef LLIST_H__
#define LLIST_H__

#define LLIST_FORWARD 1
#define LLIST_BACKWARD 2

typedef void llist_op(const void *);
typedef int llist_cmp(const void *, const void *);

struct llist_node_st
{
//    void *data;
    struct llist_node_st *prev;
    struct llist_node_st *next;    
    char data[1];
};

typedef struct
{
    int size;
    struct llist_node_st head;
}LLIST;

LLIST *llist_create(int initsize);

int llist_insert(LLIST *, const void *data, int mode);

void *llist_find(LLIST *, const void *key, llist_cmp *);

int llist_delete(LLIST *, const void *key, llist_cmp *);

int llist_fetch(LLIST *, const void *key, llist_cmp *, void *data);// 删除节点并返回

void llist_travel(LLIST *, llist_op *op);// 遍历

void llist_destory(LLIST *);

#endif
