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
#include "ntp.h"
#include "AbstractModule.h"

class NetworkModule : public AbstractModule<std::string> {
public:
    NetworkModule();
    NetworkModule(const std::string& server_ip, int server_port);
    ~NetworkModule();
    bool init() override;
    void deinit() override;
    void send_data(const std::string& data);

private:
    void run(); // 类运行线程
    void ntp_thread_func();     // NTP 线程
    
    void receive_func();        // 接收数据
    void connect_to_server();   // 连接到服务器

    bool network_run_;          // 网络运行标志
    bool is_connected_;         // 是否连接到服务器
    std::mutex mtx_network;     // 网络互斥锁
    
    std::string server_ip;      // 服务器 IP 地址
    int server_port;            // 服务器端口
    int sockfd;                 // 套接字文件描述符

    std::thread run_thread;     // 运行线程
    std::thread ntp_thread;     // NTP 线程

    // 连接线程及其同步机制
    std::mutex mtx_connect;
    std::condition_variable cond_var_connect;

    // NTP 同步机制
    std::mutex mtx_ntp;
    std::condition_variable cond_var_ntp;

    int ntp_enabled;
};