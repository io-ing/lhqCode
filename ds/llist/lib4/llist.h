#ifndef LLIST_H__
#define LLIST_H__

// 没有必要写 struct llist_head_st 总不能 include llist.c, 所以可以骗一下用户
// typedef struct llist_head_st LLIST;
typedef void LLIST;

#define LLIST_FORWARD 1
#define LLIST_BACKWARD 2

typedef void llist_op(const void *);
typedef int llist_cmp(const void *, const void *);

LLIST *llist_create(int initsize);

int llist_insert(LLIST *, const void *data, int mode);

void *llist_find(LLIST *, const void *key, llist_cmp *);

int llist_delete(LLIST *, const void *key, llist_cmp *);

int llist_fetch(LLIST *, const void *key, llist_cmp *, void *data);// 删除节点并返回

void llist_travel(LLIST *, llist_op *op);// 遍历

void llist_destory(LLIST *);

#endif
