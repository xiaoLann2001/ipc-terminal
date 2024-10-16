#include "net.h"

void * cli_data_handler(void * arg);

int main() {
    int server_fd;
    struct sockaddr_in server_address;
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

    int new_fd;
    struct sockaddr_in client_address;
    struct cli_info *cli;
    while (1) {
        int ret = -1;
        if ((cli = (struct cli_info *)malloc((sizeof(struct cli_info)))) == NULL) {
            perror("malloc");
            continue;
        }
        
        // 接受客户端连接
        if ((cli->cli_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            free(cli);
            continue;
        }

        // 输出客户端IP地址和端口号
        if (NULL == inet_ntop(AF_INET, (void *)&client_address.sin_addr, cli->cli_addr, sizeof(cli->cli_addr))) {
            perror("get clinet address");
            close(cli->cli_fd);
            free(cli);
            continue;
        }
        cli->cli_port = ntohs(client_address.sin_port);
        printf("Client connected from : (%s : %d)\n", cli->cli_addr, cli->cli_port);

        // 创建一个线程来处理客户端数据
        pthread_t tid;
        if (pthread_create(&tid, NULL, cli_data_handler, (void *)cli)) {
            perror("pthread_create");
            close(cli->cli_fd);
            free(cli);
            break;
        }

        // 使线程分离以便自动释放资源
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}

void * cli_data_handler(void * arg) {
    struct cli_info *cli = (struct cli_info *)arg;
    int new_fd = cli->cli_fd;
    char addr[INET_ADDRSTRLEN];
    int port = cli->cli_port;

    strncpy(addr, cli->cli_addr, sizeof(addr) - 1);

    // 处理客户端请求
    int ret = -1;
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        bzero(buffer, sizeof(buffer));
        // 读取客户端发送的数据
        do {
            ret = read(new_fd, buffer, BUFFER_SIZE-1);
        } while (ret < 0 && EINTR == errno);
        if (ret < 0) {
            perror("read from client");
            break;
        }
        if (0 == ret) {
            printf("client(%s : %d) is disconncted. Exiting...\n", addr, port);
            break;  // 对方已关闭连接
        }
        
        printf("Message from client(%s : %d): %s\n", addr, port, buffer);
        
        // 处理请求
        buffer[strcspn(buffer, "\n")] = '\0';
        strncat(buffer, " ---processed by server", sizeof(buffer) - strlen(buffer) - 1);

        // 响应请求
        write(new_fd, buffer, strlen(buffer));
    }
    // 关闭套接字
    close(new_fd);
    free(cli);
    return NULL;
}
