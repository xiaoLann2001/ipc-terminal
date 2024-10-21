#include "Pantilt/pwm.h"

#define SYSFS_PWM_PATH "/sys/class/pwm/pwmchip"

// Helper function to write to a sysfs file
static int sysfs_write(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open sysfs file");
        return -1;
    }
    write(fd, value, strlen(value));
    close(fd);
    return 0;
}

// Helper function to read from a sysfs file
static int sysfs_read(const char *path, char *value, size_t size) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open sysfs file");
        return -1;
    }
    read(fd, value, size);
    close(fd);
    return 0;
}

// Helper to generate sysfs path for PWM
void pwm_generate_path(char *path, int pwm_num, const char *filename) {
    sprintf(path, "%s%d/pwm0/%s", SYSFS_PWM_PATH, pwm_num, filename);
}

// Initialize PWM
int pwm_init(enum PWM_num pwm_num) {
    char path[64];
    
    // Check if PWM is already exported
    sprintf(path, "%s%d/pwm0", SYSFS_PWM_PATH, pwm_num);
    if (access(path, F_OK) == 0) {
        fprintf(stdout, "PWM%d already exported\n", pwm_num);
        return 0;
    }
    
    // Export the PWM
    sprintf(path, "%s%d/export", SYSFS_PWM_PATH, pwm_num);
    return sysfs_write(path, "0");
}

// Deinitialize (unexport) PWM
int pwm_deinit(enum PWM_num pwm_num) {
    char path[64];
    
    // Unexport the PWM
    sprintf(path, "%s%d/unexport", SYSFS_PWM_PATH, pwm_num);
    return sysfs_write(path, "0");
}

// Set PWM period (in nanoseconds)
int pwm_set_period(enum PWM_num pwm_num, unsigned int period_ns) {
    char path[64];
    char value[32];
    
    // Write period to sysfs
    pwm_generate_path(path, pwm_num, "period");
    sprintf(value, "%u", period_ns);
    return sysfs_write(path, value);
}

// Set PWM duty cycle (in nanoseconds)
int pwm_set_duty_cycle(enum PWM_num pwm_num, unsigned int duty_cycle_ns) {
    char path[64];
    char value[32];
    
    // Write duty cycle to sysfs
    pwm_generate_path(path, pwm_num, "duty_cycle");
    sprintf(value, "%u", duty_cycle_ns);
    return sysfs_write(path, value);
}

// Set PWM polarity
int pwm_set_polarity(enum PWM_num pwm_num, const char* polarity) {
    char path[64];
    
    // Write polarity to sysfs
    pwm_generate_path(path, pwm_num, "polarity");
    return sysfs_write(path, polarity);
}

// Enable or disable PWM
int pwm_enable(enum PWM_num pwm_num, int enable) {
    char path[64];
    char value[8];
    
    // Write enable/disable to sysfs
    pwm_generate_path(path, pwm_num, "enable");
    sprintf(value, "%d", enable);
    return sysfs_write(path, value);
}

// Get the current status of the PWM
int pwm_get_status(enum PWM_num pwm_num, struct Pwm_status *status) {
    char path[64];
    char value[32];

    // Read period
    pwm_generate_path(path, pwm_num, "period");
    if (sysfs_read(path, value, sizeof(value)) == 0) {
        status->period_ns = atoi(value);
    } else {
        return -1;
    }

    // Read duty cycle
    pwm_generate_path(path, pwm_num, "duty_cycle");
    if (sysfs_read(path, value, sizeof(value)) == 0) {
        status->duty_cycle_ns = atoi(value);
    } else {
        return -1;
    }

    // Read enabled status
    pwm_generate_path(path, pwm_num, "enable");
    if (sysfs_read(path, value, sizeof(value)) == 0) {
        status->enabled = atoi(value);
    } else {
        return -1;
    }

    return 0;
}
