

#ifndef _NETWORK_PRIVATE_H
#define _NETWORK_PRIVATE_H

#include <unistd.h>
#include <stdio.h>

int write_all(int sock, char *buf, int len);

int read_all(int sock, char *buf, int len);

#endif
