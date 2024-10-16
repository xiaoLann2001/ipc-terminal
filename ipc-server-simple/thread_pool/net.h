#ifndef __NET_H_
#define __NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#define SERVER_ADDR "192.168.6.30"
#define SERVER_PORT 10010
#define BUFFER_SIZE 1024
#define STR_SIZE 16

#define CMD_QUIT ".quit"
#define CMD_YES  "yes"
#define CMD_NO   "no"

#define MAX_THREADS 10
#define QUEUE_SIZE 10

struct cli_info {
    int cli_fd;
    char cli_addr[INET_ADDRSTRLEN];
    int cli_port;
};

// 线程池任务队列节点
struct task {
    struct cli_info *cli;
    struct task *next;
};

// 线程池
struct thread_pool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    struct task *queue_head;
    struct task *queue_tail;
    int thread_count;
    int queue_size;
    int shutdown;
};

#endif