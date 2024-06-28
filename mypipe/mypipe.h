#ifndef MY_PIPE__
#define MY_PIPE__

#include <stddef.h>

#define MYPIPE_READ  0x00000001UL
#define MYPIPE_WRITE 0x00000002UL

typedef void mypipe_t;

mypipe_t *mypipe_init();

int mypipe_read(mypipe_t *, char *, size_t);

int mypipe_write(mypipe_t *, const char *, size_t);

int mypipe_destroy(mypipe_t *);

int mypipe_register(mypipe_t *, int);

int mypipe_unregister(mypipe_t *, int);

#endif
