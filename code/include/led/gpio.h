#ifndef __GPIO_H_
#define __GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define GPIO_PATH "/sys/class/gpio/"

// Define GPIO numbers
enum Gpio_num {
    GPIO1_C4_d = 52,
    GPIO1_C5_d = 53,
    GPIO1_C6_d = 54,
    GPIO1_C7_d = 55,
};

// Define GPIO directions
enum Gpio_direction {
    GPIO_INPUT,
    GPIO_OUTPUT,
};

// Define GPIO edge trigger types
enum Gpio_edge {
    GPIO_EDGE_NONE,
    GPIO_EDGE_RISING,
    GPIO_EDGE_FALLING,
    GPIO_EDGE_BOTH,
};

// Define GPIO status structure
struct Gpio_status {
    enum Gpio_direction direct; // GPIO direction
    enum Gpio_edge edge;        // GPIO edge trigger mode
    int value;                  // GPIO level (value)
    int active_low;             // Logic level inversion
};

/**
 * @brief Generate the GPIO file path.
 *
 * This function generates the file path for a specific GPIO pin based on the 
 * given GPIO number and filename.
 *
 * @param path Buffer to store the generated path.
 * @param gpio_num The GPIO pin number.
 * @param filename The filename to append to the GPIO path.
 */
void gpio_generate_path(char *path, int gpio_num, const char *filename);

/**
 * @brief Initialize the GPIO pin and set its direction.
 *
 * This function exports the GPIO pin and sets its direction as either input or output.
 *
 * @param num The GPIO pin number.
 * @param direct The direction of the GPIO pin (input/output).
 * @return 0 on success, -1 on failure.
 */
int gpio_init(enum Gpio_num num, enum Gpio_direction direct);

/**
 * @brief Deinitialize (unexport) a GPIO pin.
 *
 * This function unexports the GPIO pin, making it unavailable for further use.
 *
 * @param num The GPIO pin number to unexport.
 */
void gpio_deinit(enum Gpio_num num);

/**
 * @brief Read the value of the GPIO pin.
 *
 * This function reads the current value of the specified GPIO pin (0 or 1).
 *
 * @param num The GPIO pin number.
 * @return The current value of the GPIO pin (0 or 1), or -1 on failure.
 */
int gpio_read(enum Gpio_num num);

/**
 * @brief Write a value to the GPIO pin.
 *
 * This function sets the value of the GPIO pin to either 0 (low) or 1 (high).
 *
 * @param num The GPIO pin number.
 * @param value The value to write (0 or 1).
 */
void gpio_write(enum Gpio_num num, int value);

/**
 * @brief Toggle the value of the GPIO pin.
 *
 * This function inverts the current value of the GPIO pin (0 becomes 1, and 1 becomes 0).
 *
 * @param num The GPIO pin number.
 */
void gpio_toggle(enum Gpio_num num);

/**
 * @brief Get the status of the GPIO pin.
 *
 * This function retrieves the current status of the GPIO pin, including its direction, 
 * edge trigger mode, value, and active_low setting.
 *
 * @param num The GPIO pin number.
 * @param status Pointer to a Gpio_status structure to store the retrieved status.
 * @return 0 on success, -1 on failure.
 */
int gpio_get_status(enum Gpio_num num, struct Gpio_status *status);

/**
 * @brief Set the direction of the GPIO pin.
 *
 * This function sets the GPIO pin as either input or output.
 *
 * @param num The GPIO pin number.
 * @param direct The direction to set (input/output).
 */
void gpio_set_direction(enum Gpio_num num, enum Gpio_direction direct);

/**
 * @brief Set the edge trigger type of the GPIO pin.
 *
 * This function configures the edge trigger type for the GPIO pin, such as 
 * rising, falling, or both edges.
 *
 * @param num The GPIO pin number.
 * @param edge The edge trigger type to set (rising, falling, both, or none).
 * @return 0 on success, -1 on failure.
 */
int gpio_set_edge(enum Gpio_num num, enum Gpio_edge edge);

/**
 * @brief Set the active_low configuration of the GPIO pin.
 *
 * This function configures whether the GPIO pin operates in active_low mode, where 
 * logical high is treated as low, and logical low is treated as high.
 *
 * @param num The GPIO pin number.
 * @param active_low 0 for normal mode, 1 for active_low mode.
 * @return 0 on success, -1 on failure.
 */
int gpio_set_active_low(enum Gpio_num num, int active_low);

/**
 * @brief Wait for an edge event on the GPIO pin.
 *
 * This function waits for an edge event (such as rising or falling) on the GPIO pin, 
 * with a specified timeout.
 *
 * @param num The GPIO pin number.
 * @param timeout_ms The timeout in milliseconds to wait for the edge event.
 * @return 1 if an event occurred, 0 on timeout, -1 on error.
 */
int gpio_wait_for_edge(enum Gpio_num num, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif // __GPIO_H_
