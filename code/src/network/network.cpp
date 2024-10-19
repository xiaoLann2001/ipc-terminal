#include "network/network.h"

// 构造函数：初始化网络并启动收发线程
Network::Network(const std::string& server_ip, int server_port)
    : server_ip(server_ip), server_port(server_port), is_connected(false) {

    if (connect_to_server()) {
        is_connected = true;
        receive_thread = std::thread(&Network::receive_thread_func, this);
        send_thread = std::thread(&Network::send_thread_func, this);
    } else {
        std::cerr << "Failed to connect to server" << std::endl;
    }
}

// 析构函数：关闭连接并停止线程
Network::~Network() {
    flag_quit = true;

    // 唤醒并等待线程结束
    cond_var_send.notify_all();

    if (receive_thread.joinable()) receive_thread.join();
    if (send_thread.joinable()) send_thread.join();

    // 关闭socket
    if (is_connected) {
        close(sockfd);
    }
}

// 连接到服务器
bool Network::connect_to_server() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return false;
    }

    std::cout << "Connected to server :" << server_ip << ":" << server_port << std::endl;
    return true;
}

// 发送数据到服务器
void Network::send_data(const std::string& data) {
    std::lock_guard<std::mutex> lock(mtx_send);
    send_queue.push(data);
    cond_var_send.notify_one();  // 唤醒发送线程
}

// 接收数据线程函数
void Network::receive_thread_func() {
    char buffer[1024];
    while (!flag_quit) {
        // 设置 pollfd
        struct pollfd fds[1];
        fds[0].fd = sockfd;         // 监视的套接字
        fds[0].events = POLLIN;     // 可读事件

        // 调用 poll，设置超时时间为 1000ms（1秒）
        int ret = poll(fds, 1, 1000); // 1000ms 超时

        // 检查 poll 的返回值
        if (ret < 0) {
            std::cerr << "poll() error" << std::endl;
            break;  // 发生错误，退出
        } else if (ret == 0) {
            // 超时，没有数据可读，继续循环
            continue; 
        }

        // 检查是否有可读数据
        if (fds[0].revents & POLLIN) {
            ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
                std::string received_data(buffer, bytes_received);
                std::cout << "Received: " << received_data << std::endl;
                signal_network_received.emit(received_data);
            } else if (bytes_received == 0) {
                std::cerr << "Connection closed by server" << std::endl;
                break; // 连接关闭
            } else if (bytes_received == -1) {
                std::cerr << "recv() error: " << strerror(errno) << std::endl;
                break; // 发生错误
            }
        }
    }
}

// 发送数据线程函数
void Network::send_thread_func() {
    std::string data_to_send;
    while (!flag_quit) {
        {
            std::unique_lock<std::mutex> lock(mtx_send);
            cond_var_send.wait(lock, [this] {
                return !send_queue.empty() || flag_quit;
            });

            if (flag_quit) {  
                break;
            }

            // 从队列取出数据并发送
            data_to_send = send_queue.front();
            send_queue.pop();
            lock.unlock();
        }

        ssize_t bytes_sent = send(sockfd, data_to_send.c_str(), data_to_send.size(), 0);

        if (bytes_sent == -1) {
            std::cerr << "Failed to send data" << std::endl;
        }
    }
}
