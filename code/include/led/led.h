#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio/gpio.h"

// LED Resources
enum Led_num {
    LED0 = GPIO1_C7_d,
    LED1 = GPIO1_C6_d,
};

/**
 * @brief Initialize the specified LED.
 *
 * This function initializes the given LED by configuring the associated GPIO pin.
 *
 * @param led The LED to initialize (LED0 or LED1).
 * @return 0 on success, -1 on failure.
 */
int led_init(enum Led_num led);

/**
 * @brief Deinitialize the specified LED.
 *
 * This function deinitializes the given LED by unexporting the associated GPIO pin.
 *
 * @param led The LED to deinitialize (LED0 or LED1).
 */
void led_deinit(enum Led_num led);

/**
 * @brief Turn on the specified LED.
 *
 * This function turns on the given LED by setting the GPIO pin to high.
 *
 * @param led The LED to turn on (LED0 or LED1).
 */
void led_on(enum Led_num led);

/**
 * @brief Turn off the specified LED.
 *
 * This function turns off the given LED by setting the GPIO pin to low.
 *
 * @param led The LED to turn off (LED0 or LED1).
 */
void led_off(enum Led_num led);

/**
 * @brief Toggle the specified LED.
 *
 * This function toggles the state of the given LED, changing it from on to off, or off to on.
 *
 * @param led The LED to toggle (LED0 or LED1).
 */
void led_toggle(enum Led_num led);

/**
 * @brief Blink the specified LED for a certain duration.
 *
 * This function makes the given LED blink by turning it on, waiting for the specified duration, and then turning it off.
 *
 * @param led The LED to blink (LED0 or LED1).
 * @param duration_ms The duration in milliseconds for which the LED should stay on before turning off.
 */
void led_blink(enum Led_num led, int duration_ms);

#ifdef __cplusplus
}
#endif

#endif // __LED_H_
