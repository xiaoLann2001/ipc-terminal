#ifndef NETWORK_H
#define NETWORK_H

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

#include "Signal/Signal.h"

class Network {
public:
    Network(const std::string& server_ip, int server_port);
    ~Network();

    // 从Control类接收数据并放入发送队列
    void send_data(const std::string& data);

    Signal<std::string> signal_network_received;

private:
    void receive_thread_func();  // 接收数据线程
    void send_thread_func();     // 发送数据线程

    bool flag_quit = false;
    
    std::string server_ip;
    int server_port;
    int sockfd;
    bool is_connected;

    std::thread receive_thread;
    std::thread send_thread;

    // 发送队列及其同步机制
    std::queue<std::string> send_queue;
    std::mutex mtx_send;
    std::condition_variable cond_var_send;

    // 初始化网络连接
    bool connect_to_server();
};

#endif // NETWORK_H