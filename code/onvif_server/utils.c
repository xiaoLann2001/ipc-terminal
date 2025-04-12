#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int get_local_ip(const char *interface_name, char ip_buffer[IP_ADDR_LENGTH]) {
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return -1;

    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        close(fd);
        return -1;
    }

    close(fd);
    struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
    strncpy(ip_buffer, inet_ntoa(ipaddr->sin_addr), IP_ADDR_LENGTH - 1);
    return 0;
}

int get_local_mac(const char *iface, unsigned char mac[MAC_ADDR_LENGTH]) {
    int fd;
    struct ifreq ifr;

    if (iface == NULL || mac == NULL) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        return -1;
    }

    close(fd);

    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    return 0;
}