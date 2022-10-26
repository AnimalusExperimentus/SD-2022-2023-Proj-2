#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include <netinet/in.h>

struct rtree_t {  
    struct sockaddr_in server;
    int sockfd;
};

#endif //_CLIENT_STUB_PRIVATE_H