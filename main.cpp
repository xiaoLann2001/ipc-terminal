#include "ipc_global.h"
#include "led/led.h"
#include "led/gpio.h"

int main() {

    char str[32] = HELLO_STR;
    strcat(str, LED_STR);
    fprintf(stdout, str);

    char path[64];
    gpio_generate_path(path, 54, "test");
    fprintf(stdout, path);

    // gpio_init(GPIO1_C6_d, GPIO_OUTPUT);

    return 0;
}