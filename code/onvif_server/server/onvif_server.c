#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "soapH.h"
#include "wsdd.nsmap"
#include "soapStub.h"

#include "onvif_config.h"

static pthread_t ws_thread;
static pthread_t web_thread;
static volatile int g_onvif_exit = 0;

// WebService Discovery 线程处理函数
static void *ws_discovery_thread(void *arg) {
    struct soap soap_wsdd;
    int count = 0;

    soap_init1(&soap_wsdd, SOAP_IO_UDP | SOAP_XML_IGNORENS);
    soap_set_namespaces(&soap_wsdd, namespaces);

    if (!soap_valid_socket(soap_bind(&soap_wsdd, NULL, WS_DISCOVERY_PORT, 10))) {
        soap_print_fault(&soap_wsdd, stderr);
        return NULL;
    }

    struct ip_mreq mcast;
    mcast.imr_multiaddr.s_addr = inet_addr(ONVIF_MULTICAST_IP);
    mcast.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(soap_wsdd.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0) {
        printf("setsockopt error! error code = %d, err string = %s\n", errno, strerror(errno));
        return NULL;
    }

    while (!g_onvif_exit) {
        // 设置超时（避免阻塞退出）
        soap_wsdd.accept_timeout = 1;
        soap_wsdd.recv_timeout = 1;

        if (soap_serve(&soap_wsdd)) {
            if (soap_wsdd.errnum != 0 && soap_wsdd.errnum != SOAP_EOF) {
                soap_print_fault(&soap_wsdd, stderr);
            }
        }

        soap_destroy(&soap_wsdd);
        soap_end(&soap_wsdd);
        count++;
    }

    soap_done(&soap_wsdd);
    return NULL;
}

// WebService 服务线程处理函数
static void *web_service_thread(void *arg) {
    struct soap soap_http;
    soap_init1(&soap_http, SOAP_XML_IGNORENS | SOAP_C_UTFSTRING);
    soap_set_namespaces(&soap_http, namespaces);

    int master_socket = soap_bind(&soap_http, NULL, WEB_SERVICE_PORT, 100);
    if (!soap_valid_socket(master_socket)) {
        soap_print_fault(&soap_http, stderr);
        return NULL;
    }

    printf("Web Service Server is listening on port %d...\n", WEB_SERVICE_PORT);

    while (!g_onvif_exit) {
        soap_http.accept_timeout = 1;
        soap_http.recv_timeout = 1;

        int slave_socket = soap_accept(&soap_http);
        if (!soap_valid_socket(slave_socket)) {
            if (soap_http.errnum != 0 && soap_http.errnum != SOAP_EOF) {
                soap_print_fault(&soap_http, stderr);
            }
            continue;
        }

        if (soap_serve(&soap_http)) {
            soap_print_fault(&soap_http, stderr);
        }

        soap_destroy(&soap_http);
        soap_end(&soap_http);
    }

    soap_done(&soap_http);
    return NULL;
}

void onvif_server_init() {
    g_onvif_exit = 0;

    if (pthread_create(&ws_thread, NULL, ws_discovery_thread, NULL) != 0) {
        perror("Failed to create WS-Discovery thread");
        exit(1);
    }

    if (pthread_create(&web_thread, NULL, web_service_thread, NULL) != 0) {
        perror("Failed to create WebService thread");
        exit(1);
    }

    printf("ONVIF server initialized successfully.\n");
}

void onvif_server_deinit() {
    g_onvif_exit = 1;

    pthread_join(ws_thread, NULL);
    pthread_join(web_thread, NULL);

    printf("ONVIF server deinitialized.\n");
}
