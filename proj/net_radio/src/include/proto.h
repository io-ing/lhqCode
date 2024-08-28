#ifndef PROTO_H__
#define PROTO_H__

#include "site_type.h"

#define DEFAULT_MULTI_GROUP     "224.2.2.2"
#define DEFAULT_RECEICE_PROT    "1989"

#define LIST_CHANNEL_ID 0

#define MIN_CHANNEL_ID  1
#define CHANNEL_NUMBER  100
#define MAX_CHANNEL_ID  (MIN_CHANNEL_ID + CHANNEL_NUMBER - 1)


struct msg_channel_st
{
    /* must be MIN_CHANNEL_ID ~ MAX_CHANNEL_ID */
    channel_id_t id;
    uint8_t data[1];
}__attribute__((packed));

/* 20~60 ip 包长度，8 udp 包长度 */
#define MSG_CHANNEL_MAX (65536 - 20 - 8)
#define MSG_DATA_MAX    (MSG_CHANNEL_MAX - sizeof(channel_id_t));

#define MSG_LIST_MAX    (65536 - 20 - 8)
#define MSG_ENTRY_MAX   (MSG_LIST_MAX - sizeof(channel_id_t))

struct msg_listentry_st
{
    channel_id_t channel_id;
    uint8_t description[1];
}__attribute__((packed));

Sstruct msg_list_st
{
    /* must be LIST_CHANNEL_ID */
    channel_id_t channel_id;
    struct msg_listentry_st entry[1];
}__attribute__((packed));

#endif /* PROTO_H__ */
