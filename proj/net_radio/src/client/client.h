#ifndef CLIENT_H__
#define CLIENT_H__

#include "proto.h"

struct client_conf_st
{
    char *recvport;
    char *mutilgroup;
    char *player_cmd;
};

extern struct client_conf_st client_conf;

#endif /* CLIENT_H__ */
