
#include "Network/Network.h"
#include "Control/Control.h"

bool quit = false;

static void sigterm_handler(int sig) {
    fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
    quit = true;
}

int main(int argc, char *argv[]) {
    // 注册信号处理函数，捕获 SIGINT 和 SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    std::string ip_address = argv[1];
    int port = atoi(argv[2]);
    Network *network = new Network(ip_address, port);
    Control *control = new Control();
    network->signal_network_received.connect(control, &Control::onNetworkReceived);

    while (!quit) {
        // network->send_data("Hello, server");
        sleep(1);
    }

    delete network;
    delete control;

    return 0;
}
