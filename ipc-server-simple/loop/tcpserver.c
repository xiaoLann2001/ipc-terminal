#include "net.h"

int process(int new_fd);

int main() {
    int server_fd, new_fd;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int addrlen = sizeof(server_address);
    int opt = 1;

    printf("server(%s:%d) is starting...\n", SERVER_ADDR, SERVER_PORT);

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置端口可重用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsocketopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 绑定IP和端口
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;    // 绑定任意IP 
    server_address.sin_port = htons(SERVER_PORT);
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 监听端口
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 接受客户端连接
    if ((new_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // 输出客户端IP地址和端口号
        char ipv4_addr[16];
        if (NULL == inet_ntop(AF_INET, (void *)&client_address.sin_addr, ipv4_addr, sizeof(client_address))) {
            perror("get clinet address");
        }
        printf("Client connected from : (%s : %d)\n", ipv4_addr, ntohs(client_address.sin_port));

        // 处理客户端请求
        if (process(new_fd) < 0) {
            break;
        }
    }

    // 关闭套接字
    close(new_fd);
    close(server_fd);
    return 0;
}

int process(int new_fd) {
    int ret = -1;
    char buffer[BUFFER_SIZE] = {0};

    // 读取客户端发送的数据
    do {
        ret = read(new_fd, buffer, BUFFER_SIZE-1);
    } while (ret < 0 && EINTR == errno);
    if (ret < 0) {
        perror("read from client");
        return -1;
    }
    if (0 == ret) {
        perror("client is disconncted. Exiting...");
        return -1;  // 对方已关闭连接
    }
    
    printf("Message from client: %s\n", buffer);
    
    // 处理请求
    buffer[strcspn(buffer, "\n")] = '\0';
    strcat(buffer, " ---processed by server");

    // 响应请求
    write(new_fd, buffer, strlen(buffer));
    return 0;
}
