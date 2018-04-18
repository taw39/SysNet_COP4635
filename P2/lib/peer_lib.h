#ifndef _PEER_LIB_H
#define _PEER_LIB_H

#include <stdio.h>

#define PORT 60001
#define MAX_BUFFER 100000
#define CONV_LENGTH 4096

FILE *fp;
char *file_path;
char *buffer;
int peer_port;
int host_port;
int message_count;
int debug = 0;

void *logicRing(void *vargp);

#endif