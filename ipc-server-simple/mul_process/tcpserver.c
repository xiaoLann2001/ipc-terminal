#include "net.h"

void cli_data_handler(void * arg);

void sig_child_handler(int signo) {
    if (SIGCHLD == signo) {
        waitpid(-1, NULL, WNOHANG);
    }
}

int main() {
    int server_fd;
    struct sockaddr_in server_address;
    int addrlen = sizeof(server_address);
    int opt = 1;

    printf("server(%s:%d) is starting...\n", SERVER_ADDR, SERVER_PORT);

    // 处理子进程发来的信号，回收僵尸进程
    signal(SIGCHLD, sig_child_handler);

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
    struct cli_info cli;
    while (1) {
        int ret = -1;
        
        // 接受客户端连接
        if ((cli.cli_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            break;
        }

        // 输出客户端IP地址和端口号
        if (NULL == inet_ntop(AF_INET, (void *)&client_address.sin_addr, cli.cli_addr, sizeof(client_address))) {
            perror("get clinet address");
            exit(EXIT_FAILURE);
        }
        cli.cli_port = ntohs(client_address.sin_port);
        printf("Client connected from : (%s : %d)\n", cli.cli_addr, cli.cli_port);

        // 创建一个子进程来处理客户端数据
        if ((ret = fork()) == -1)
        {
            perror("fork");
            break;
        } 

        if (0 == ret) { // 子进程
            close(server_fd);

            cli_data_handler(&cli);

            return 0;

        } else {        // 父进程
            close(new_fd);
        }  
    }

    close(server_fd);
    return 0;
}

void cli_data_handler(void * arg) {
    struct cli_info cli = *(struct cli_info *)arg;
    int new_fd = cli.cli_fd;
    char addr[INET_ADDRSTRLEN];
    int port = cli.cli_port;

    strcpy(addr, cli.cli_addr);

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
        strcat(buffer, " ---processed by server");

        // 响应请求
        write(new_fd, buffer, strlen(buffer));
    }
    // 关闭套接字
    close(new_fd);
}
