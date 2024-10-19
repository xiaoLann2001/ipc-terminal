#include "led/led.h"
#include <unistd.h>  // for usleep()

// Initialize LED (exports the GPIO pins)
int led_init(enum Led_num led) {
    // Initialize both LEDs as GPIO outputs
    if (gpio_init(led, GPIO_OUTPUT) == -1) {
        fprintf(stderr, "Failed to initialize LED: %d\n", led);
        return -1;
    }
    printf("LED %d initialized\n", led);
    return 0;
}

// Deinitialize LED (unexport the GPIO pins)
void led_deinit(enum Led_num led) {
    led_off(led);
    gpio_deinit(led);
}

// Turn on the specified LED
void led_on(enum Led_num led) {
    gpio_write(led, 0);  // Set GPIO to high (LED on)
}

// Turn off the specified LED
void led_off(enum Led_num led) {
    gpio_write(led, 1);  // Set GPIO to low (LED off)
}

// Toggle the specified LED state
void led_toggle(enum Led_num led) {
    gpio_toggle(led);  // Invert the current LED state
}

// Blink the LED with a specified duration in us
void led_blink(enum Led_num led, int duration_us) {
    led_on(led);  // Turn on the LED
    usleep(duration_us);  // Sleep for the duration
    led_off(led);  // Turn off the LED
}
