#ifndef MY_PIPE__
#define MY_PIPE__

#define MYPIPE_READER 0x00000001ULL
#define MYPIPE_WRITER 0x00000002ULL

typedef void mypipe_t;

mypipe_t *mypipe_init();

int mypipe_read(mypipe_t *, void *, size_t);

int mypipe_write(mypipe_t *, void *, size_t);

void mypipe_destroy(mypipe_t *);

int mypipe_register(mypipe_t *, int);

int mypipe_unregister(mypipe_t *, int);

#endif
