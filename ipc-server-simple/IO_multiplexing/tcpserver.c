#include "net.h"

void cli_data_handler(void * arg);

int main() {
    int server_fd, newfd;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    struct cli_info cli[MAX_CLIENT];
    int addrlen = sizeof(server_address);
    int opt = 1;

    int maxfd;
    int activity; 
    fd_set r_set;
    struct timeval tout;

    printf("Server(%s:%d) is starting...\n", SERVER_ADDR, SERVER_PORT);

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

    // 初始化cli结构体数组
    for (int i = 0; i < MAX_CLIENT; i++) {
        cli[i].cli_fd = -1;
    } 

    while (1) {
        // 将读集合清零
        FD_ZERO(&r_set);

        // 将服务器socket和已建立连接的客户端加入读集合
        FD_SET(server_fd, &r_set);
        maxfd = server_fd;
        tout.tv_sec = 5;
        tout.tv_usec = 0;
        for (int i = 0; i < MAX_CLIENT; i++) {
            if (cli[i].cli_fd > 0) {
                FD_SET(cli[i].cli_fd, &r_set);
            }
            if (maxfd < cli[i].cli_fd) {
                maxfd = cli[i].cli_fd;
            }
        }

        activity = select(maxfd + 1, &r_set, NULL, NULL, &tout);
        if (activity < 0 && errno != EINTR) {
            perror("select error");
            break;
        } else if (activity == 0) {
            printf("Timeout occurred! No data after 5 seconds.\n");
            continue;
        }

        // 有新客户端连接
        if (FD_ISSET(server_fd, &r_set)) {
            // 接受客户端连接
            if ((newfd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) {
                perror("accept failed");
                continue;
            }

            // 将新的客户端连接信息加入cli结构体数组
            for (int i = 0; i < MAX_CLIENT; i++) {
                if (cli[i].cli_fd == -1) {
                    cli[i].cli_fd = newfd;

                    // 输出客户端IP地址和端口号
                    if (NULL == inet_ntop(AF_INET, (void *)&client_address.sin_addr, cli[i].cli_addr, sizeof(client_address))) {
                        perror("get clinet address");
                        continue;
                    }
                    cli[i].cli_port = ntohs(client_address.sin_port);
                    printf("Client connected from : (%s : %d)\n", cli[i].cli_addr, cli[i].cli_port);

                    break;
                }
            }
        }

        // 已连接的客户端有数据
        for (int i = 0; i < MAX_CLIENT; i++) {
            if (FD_ISSET(cli[i].cli_fd, &r_set)) {
                // 处理客户端请求
                cli_data_handler(&cli[i]);
            }
        }
    }

    // 关闭套接字
    close(server_fd);
    return 0;
}

void cli_data_handler(void * arg) {
    // 这行代码中的cli只是函数栈区的数据，不能改变客户端结构体数组的值
    // struct cli_info cli = *(struct cli_info *)arg;
    struct cli_info *cli = (struct cli_info *)arg;
    int new_fd = cli->cli_fd;
    char addr[INET_ADDRSTRLEN];
    int port = cli->cli_port;

    strcpy(addr, cli->cli_addr);

    // 处理客户端请求
    int ret = -1;
    char buffer[BUFFER_SIZE];

    bzero(buffer, sizeof(buffer));

    // 读取客户端发送的数据
    do {
        ret = read(new_fd, buffer, BUFFER_SIZE-1);
    } while (ret < 0 && EINTR == errno);
    if (ret < 0) {
        perror("read from client");
        close(new_fd);      // 关闭套接字
        cli->cli_fd = -1;    // 还原cli结构体
    }
    if (0 == ret) {         // 对方已关闭连接
        printf("Client(%s:%d) is disconncted. Exiting...\n", addr, port);
        close(new_fd);      // 关闭套接字
        cli->cli_fd = -1;    // 还原cli结构体
    }
    
    printf("Message from client(%s : %d): %s\n", addr, port, buffer);
    
    // 处理请求
    buffer[strcspn(buffer, "\n")] = '\0';
    strcat(buffer, " ---Processed by Server");

    // 响应请求
    ret = -1;
    do {
        ret = write(new_fd, buffer, strlen(buffer));
    } while (ret < 0 && EINTR == errno);
}
