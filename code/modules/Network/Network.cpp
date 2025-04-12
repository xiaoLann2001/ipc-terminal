#include "Network.h"

/**
 * @brief Network 类构造函数。
 */
Network::Network() {
    {
        std::lock_guard<std::mutex> lock(mtx_network);
        network_run_ = true;
        is_connected_ = false;
    }

    server_ip = rk_param_get_string("network:server_ip", "127.0.0.1");
    server_port = rk_param_get_int("network:port", 8888);

    run_thread = std::thread(&Network::run, this);
}

/**
 * @brief Network 类构造函数。
 * 
 * @param server_ip 服务器 IP 地址。
 * @param server_port 服务器端口。
 */
Network::Network(const std::string& server_ip, int server_port)
    : server_ip(server_ip), server_port(server_port), is_connected_(false) {

    run_thread = std::thread(&Network::run, this);
}

/**
 * @brief Network 类析构函数。
 */
Network::~Network() {
    // 设置退出标志，通知线程退出
    {
        std::lock_guard<std::mutex> lock(mtx_network);
        network_run_ = false;
        is_connected_ = false;
    }

    // 唤醒发送线程，避免发送线程因条件变量阻塞
    cond_var_ntp.notify_all();
    cond_var_send.notify_all();
    cond_var_connect.notify_all();
    cond_var_run.notify_all();

    if (receive_thread.joinable()) {
        receive_thread.join();  // 等待接收线程完成
    }

    if (send_thread.joinable()) {
        send_thread.join();  // 等待发送线程完成
    }

    if (ntp_enabled && ntp_thread.joinable()) {
        ntp_thread.join();  // 等待 NTP 线程完成
    }

    if (run_thread.joinable()) {
        run_thread.join();  // 等待运行线程完成
    }

    if (sockfd > 0) {
        close(sockfd);  // 关闭套接字
    }

    LOG_DEBUG("Network disconnected and resources released.\n");
}

/**
 * @brief Network 类运行线程。
 */
void Network::run() {
    // connect_to_server();
    // receive_thread = std::thread(&Network::receive_thread_func, this);
    // send_thread = std::thread(&Network::send_thread_func, this);
    ntp_enabled = rk_param_get_int("network.ntp:enable", 0);
    LOG_DEBUG("NTP enable: %d\n", ntp_enabled);
    if (ntp_enabled) {
        ntp_thread = std::thread(&Network::ntp_thread_func, this);
    }

    sockfd = -1;
    while (network_run_) {
        if (!is_connected_) {  // 如果服务器关闭
            LOG_DEBUG("Server disconnected, reconnecting...\n");

            if (sockfd > 0) close(sockfd);          // 关闭连接
            sockfd = -1;

            connect_to_server();

            if (!network_run_) break;

            if (receive_thread.joinable()) receive_thread.join();
            if (send_thread.joinable()) send_thread.join();

            receive_thread = std::thread(&Network::receive_thread_func, this);
            send_thread = std::thread(&Network::send_thread_func, this);
        } else {
            // 每秒检查一次连接状态
            // LOG_DEBUG("Checking connection...\n");
            std::unique_lock<std::mutex> lock(mtx_run);
            cond_var_run.wait_for(lock, std::chrono::seconds(1), [this] { return !is_connected_; });
        }
    }

    LOG_DEBUG("Network thread exited\n");
}

/**
 * @brief 连接到服务器。
 */
void Network::connect_to_server() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        LOG_ERROR("Error creating socket\n");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    while (network_run_) {
        LOG_DEBUG("Connecting to server: %s:%d\n", server_ip.c_str(), server_port);
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
            LOG_DEBUG("Connected to server: %s:%d\n", server_ip.c_str(), server_port);
            {
                std::lock_guard<std::mutex> lock(mtx_network);
                is_connected_ = true;
            }
            break;
        } else {
            LOG_ERROR("Connection failed, retrying in 30 seconds...\n");
            // 等待 30 秒或直到 network_run_ 被设置为 false
            std::unique_lock<std::mutex> lock(mtx_connect);
            if (cond_var_connect.wait_for(lock, std::chrono::seconds(30), [this] { return !network_run_; })) {
                LOG_DEBUG("Quit flag set. Exiting connection attempt.\n");
                break;
            }
        }
    }
}

/**
 * @brief 发送数据到服务器。
 * 
 * @param data 要发送的数据。
 */
void Network::send_data(const std::string& data) {
    if (!is_connected_) {
        LOG_ERROR("Not connected to server\n");
        return;
    }

    std::lock_guard<std::mutex> lock(mtx_send);
    send_queue.push(data);
    cond_var_send.notify_one();  // 唤醒发送线程
}

/**
 * @brief 接收数据线程函数。
 */
void Network::receive_thread_func() {
    char buffer[1024];
    while (network_run_ && is_connected_) {
        // 设置 pollfd
        struct pollfd fds[1];
        fds[0].fd = sockfd;         // 监视的套接字
        fds[0].events = POLLIN;     // 可读事件

        // 调用 poll，设置超时时间为 1000ms（1秒）
        int ret = poll(fds, 1, 1000); // 1000ms 超时

        // 检查 poll 的返回值
        if (ret < 0) {
            LOG_ERROR("poll() error\n");
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
                LOG_DEBUG("Received: %s\n", received_data.c_str());
                signal_network_received.emit(received_data);
            } else if (bytes_received == 0) {   // 服务器关闭连接
                LOG_ERROR("Connection closed by server\n");
                {
                    std::lock_guard<std::mutex> lock(mtx_network);
                    is_connected_ = false;   // 标记为未连接
                    cond_var_run.notify_all();  // 通知 RUN 线程尝试重新连接
                }
                break; // 连接关闭
            } else if (bytes_received == -1) {
                LOG_ERROR("recv() error: %s\n", strerror(errno));
                {
                    std::lock_guard<std::mutex> lock(mtx_network);
                    is_connected_ = false;   // 标记为未连接
                    cond_var_run.notify_all();  // 通知 RUN 线程尝试重新连接
                }
                break; // 发生错误
            }
        }
    }

    LOG_DEBUG("Receive thread exited\n");
}

/**
 * @brief 发送数据线程函数。
 */
void Network::send_thread_func() {
    std::string data_to_send;
    while (network_run_ && is_connected_) {
        {
            std::unique_lock<std::mutex> lock(mtx_send);
            cond_var_send.wait(lock, [this] {
                return !send_queue.empty() || !network_run_ || !is_connected_;
            });

            if (!network_run_ || !is_connected_) {  
                break;
            }

            // 从队列取出数据并发送
            data_to_send = send_queue.front();
            send_queue.pop();
            lock.unlock();
        }

        ssize_t bytes_sent = send(sockfd, data_to_send.c_str(), data_to_send.size(), 0);
        if (bytes_sent == -1) {
            LOG_ERROR("Failed to send data\n");
        }
    }

    LOG_DEBUG("Send thread exited\n");
}

/**
 * @brief NTP 线程函数。
 */
void Network::ntp_thread_func() {
    int refresh_time_s = rk_param_get_int("network.ntp:refresh_time_s", 60);
	const char *ntp_server = rk_param_get_string("network.ntp:ntp_server", "119.28.183.184");
	LOG_DEBUG("refresh_time_s is %d, ntp_server is %s\n", refresh_time_s, ntp_server);
    // rkipc_ntp_update(ntp_server);

    while (network_run_) {
        // 每隔 60 秒发送一次 NTP 请求
        std::unique_lock<std::mutex> lock(mtx_ntp);
        cond_var_ntp.wait_for(lock, std::chrono::seconds(refresh_time_s), [this] { return !network_run_; });

        // rkipc_ntp_update(ntp_server);
    }

    LOG_DEBUG("NTP thread exited\n");
}
