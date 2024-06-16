#ifndef MYSEM_H__
#define MYSEM_H__

typedef void mysem_t;

/* todo 为什么这个要起名称 */
mysem_t *mysem_init(int initval);

int mysem_add(mysem_t *, int);

int mysem_sub(mysem_t *, int);

int mysem_destroy(mysem_t *);

#endif
