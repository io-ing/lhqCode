#ifndef SQLIST_H__
#define SQLIST_H__

#define DATASIZE 1024

typedef int datatype;

typedef struct node_st
{
    datatype data[DATASIZE];
    int last;
}sqlist;

sqlist *sqlist_create();

int sqlist_create1(sqlist **);

int sqlist_insert(sqlist *, int, datatype *);

int sqlist_delete(sqlist *, int);

int sqlist_find(sqlist *, datatype *);

int sqlist_isempty(sqlist *);

int sqlist_setempty(sqlist *);

int sqlist_getNum(sqlist *);

int sqlist_destory(sqlist *);

int sqlist_union(sqlist *,sqlist *);



int sqlist_display(sqlist *);
#endif
