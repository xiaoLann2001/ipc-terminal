
#include "Network/Network.h"
#include "Control/Control.h"

bool quit = false;

static void sigterm_handler(int sig) {
    fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
    quit = true;
}

int main() {
    // 注册信号处理函数，捕获 SIGINT 和 SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    Network *network = new Network("192.168.5.42", 8888);
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
