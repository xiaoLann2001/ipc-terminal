
#include "global.h"
#include "Network/Network.h"
#include "Control/Control.h"

int rkipc_log_level = LOG_LEVEL_DEBUG;
char ini_path[] = "ipc-terminal.ini";

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

    rk_param_init(ini_path);

    std::string ip_address = argv[1];
    int port = atoi(argv[2]);
    // Network *network = new Network(ip_address, port);
    Network *network = new Network();
    Control *control = new Control();
    network->signal_network_received.connect(control, &Control::onNetworkReceived);

    while (!quit) {
        // LOG_ERROR("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_WARN("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_INFO("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_DEBUG("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));

        // network->send_data("Hello, server");
        sleep(1);
    }

    rk_param_deinit();

    delete network;
    delete control;

    return 0;
}
