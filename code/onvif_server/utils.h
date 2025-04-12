#ifndef UTILS_H
#define UTILS_H

#define MAC_ADDR_LENGTH 6
#define IP_ADDR_LENGTH 16

#ifdef __cplusplus
extern "C" {
#endif

int get_local_ip(const char *interface_name, char ip_buffer[IP_ADDR_LENGTH]);

int get_local_mac(const char *iface, unsigned char mac[MAC_ADDR_LENGTH]);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
