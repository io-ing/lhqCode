#ifndef THREAD_LIST_H__
#define THREAD_LIST_H__

#include "media_lib.h"

int thread_list_create(struct media_lib_list_entry_st *list, int list_size);

int thread_list_destroy();

#endif /* THREAD_LIST_H__ */
