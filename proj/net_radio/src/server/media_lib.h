#ifndef MEDIA_LIB_H__
#define MEDIA_LIB_H__

#include "site_type.h"

struct media_lib_list_entry_st
{
    channel_id_t channel_id;
    char *description;
};

int media_lib_get_channel_list(struct media_lib_list_entry_st **list, int *list_size);

int media_lib_destroy_channel_list(struct media_lib_list_entry_st *list);

ssize_t media_lib_read_channel(int channel_id, void *buffer, size_t size);

#endif /* MEDIA_LIB_H__ */
