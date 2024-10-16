#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @brief 初始化网络模块，创建套接字并连接到指定的IP和端口
 * 
 * @param ip 服务器的IP地址
 * @param port 服务器的端口号
 * @return 成功时返回套接字描述符，失败时返回-1
 */
int network_init(const char *ip, int port);

/**
 * @brief 发送数据到服务器
 * 
 * @param sockfd 套接字描述符
 * @param data 要发送的数据
 * @param size 数据的大小（字节数）
 * @return 成功发送的字节数，失败时返回-1
 */
int network_send(int sockfd, const void *data, size_t size);

/**
 * @brief 接收从服务器发送的数据
 * 
 * @param sockfd 套接字描述符
 * @param buffer 存储接收数据的缓冲区
 * @param size 缓冲区的大小（字节数）
 * @return 成功接收的字节数，失败时返回-1
 */
int network_recv(int sockfd, void *buffer, size_t size);

/**
 * @brief 关闭与服务器的连接
 * 
 * @param sockfd 套接字描述符
 */
void network_close(int sockfd);

#endif // NET_H

