#include "network/network.h"

int network_init(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    // 创建套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // 将IP地址转换为网络地址格式
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        return -1;
    }

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return -1;
    }

    printf("Connected to %s:%d\n", ip, port);
    return sockfd;
}

int network_send(int sockfd, const void *data, size_t size) {
    ssize_t sent = send(sockfd, data, size, 0);
    if (sent < 0) {
        perror("Send error");
    }
    return sent;
}

int network_recv(int sockfd, void *buffer, size_t size) {
    ssize_t received = recv(sockfd, buffer, size, 0);
    if (received < 0) {
        perror("Receive error");
    }
    return received;
}

void network_close(int sockfd) {
    close(sockfd);
    printf("Connection closed\n");
}
