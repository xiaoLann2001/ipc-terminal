#ifndef __NET_H_
#define __NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>

#define SERVER_ADDR "192.168.6.30"
#define SERVER_PORT 10010
#define BUFFER_SIZE 1024
#define STR_SIZE 16

#define MAX_CLIENT 64

#define CMD_QUIT ".quit"
#define CMD_YES  "yes"
#define CMD_NO   "no"

struct cli_info {
    int cli_fd;
    char cli_addr[INET_ADDRSTRLEN];
    int cli_port;
};

#endif