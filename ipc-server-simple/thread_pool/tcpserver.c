#include "net.h"

// 全局线程池
struct thread_pool pool;

void * thread_pool_worker(void *arg);
void thread_pool_add_task(struct cli_info *cli);

void * cli_data_handler(void * arg);

int main() {
    int server_fd;
    struct sockaddr_in server_address;
    int addrlen = sizeof(server_address);
    int opt = 1;

    // 初始化线程池
    pool.thread_count = MAX_THREADS;
    pool.queue_size = 0;
    pool.shutdown = 0;
    pool.threads = malloc(sizeof(pthread_t) * MAX_THREADS);
    pool.queue_head = NULL;
    pool.queue_tail = NULL;
    pthread_mutex_init(&(pool.lock), NULL);
    pthread_cond_init(&(pool.notify), NULL);

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&(pool.threads[i]), NULL, thread_pool_worker, NULL);
    }

    printf("server(%s:%d) is starting...\n", SERVER_ADDR, SERVER_PORT);

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置端口可重用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
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

    struct sockaddr_in client_address;
    struct cli_info *cli;
    while (1) {
        if ((cli = malloc(sizeof(struct cli_info))) == NULL) {
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
            perror("get client address");
            close(cli->cli_fd);
            free(cli);
            continue;
        }
        cli->cli_port = ntohs(client_address.sin_port);
        printf("Client connected from : (%s : %d)\n", cli->cli_addr, cli->cli_port);

        // 将任务添加到线程池队列
        thread_pool_add_task(cli);
    }

    close(server_fd);
    return 0;
}

void * thread_pool_worker(void *arg) {
    while (1) {
        struct task *task;
        
        pthread_mutex_lock(&(pool.lock));

        while ((pool.queue_size == 0) && (!pool.shutdown)) {
            pthread_cond_wait(&(pool.notify), &(pool.lock));
        }

        if ((pool.queue_size == 0) && (pool.shutdown)) {
            pthread_mutex_unlock(&(pool.lock));
            pthread_exit(NULL);
        }

        task = pool.queue_head;
        pool.queue_head = task->next;
        pool.queue_size--;

        pthread_mutex_unlock(&(pool.lock));

        if (task != NULL) {
            cli_data_handler((void *)task->cli);
            free(task);
        }
    }
}

void thread_pool_add_task(struct cli_info *cli) {
    struct task *new_task = (struct task *)malloc(sizeof(struct task));
    if (new_task == NULL) {
        perror("Failed to allocate memory for new task");
        free(cli);
        return;
    }
    new_task->cli = cli;
    new_task->next = NULL;

    pthread_mutex_lock(&(pool.lock));

    if (pool.queue_size == 0) {
        pool.queue_head = new_task;
        pool.queue_tail = new_task;
    } else {
        pool.queue_tail->next = new_task;
        pool.queue_tail = new_task;
    }
    pool.queue_size++;

    pthread_cond_signal(&(pool.notify));
    pthread_mutex_unlock(&(pool.lock));
}

void * cli_data_handler(void * arg) {
    struct cli_info *cli = (struct cli_info *)arg;
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
            ret = read(cli->cli_fd, buffer, BUFFER_SIZE-1);
        } while (ret < 0 && EINTR == errno);
        if (ret < 0) {
            perror("read from client");
            break;
        }
        if (0 == ret) {
            printf("client(%s : %d) is disconnected. Exiting...\n", addr, port);
            break;  // 对方已关闭连接
        }
        
        printf("Message from client(%s : %d): %s\n", addr, port, buffer);
        
        // 处理请求
        buffer[strcspn(buffer, "\n")] = '\0';
        strncat(buffer, " ---processed by server", sizeof(buffer) - strlen(buffer) - 1);

        // 响应请求
        write(cli->cli_fd, buffer, strlen(buffer));
    }
    // 关闭套接字
    close(cli->cli_fd);
    free(cli);
    return NULL;
}
