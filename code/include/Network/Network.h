#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <poll.h>
#include <cerrno>
#include <cstring>

#include "global.h"

class Network {
public:
    Network();

    Network(const std::string& server_ip, int server_port);
    
    ~Network();

    // 从Control类接收数据并放入发送队列
    void send_data(const std::string& data);

    Signal<std::string> signal_network_received;

private:
    void run(); // 类运行线程

    void receive_thread_func(); // 接收数据线程

    void send_thread_func();    // 发送数据线程

    void connect_to_server(); // 连接到服务器

    bool flag_quit = false;
    bool is_connected;
    
    std::string server_ip;
    int server_port;
    int sockfd;

    std::thread run_thread;
    std::thread receive_thread;
    std::thread send_thread;

    // 连接线程及其同步机制
    std::mutex mtx_connect;
    std::condition_variable cond_var_connect;

    // 发送队列及其同步机制
    std::queue<std::string> send_queue;
    std::mutex mtx_send;
    std::condition_variable cond_var_send;
};