#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>

// ANSI color codes
#define RESET_COLOR    "\033[0m"
#define RED_COLOR      "\033[31m"
#define YELLOW_COLOR   "\033[33m"
#define GREEN_COLOR    "\033[32m"
#define BLUE_COLOR     "\033[34m"
#define WHITE_COLOR    "\033[37m"

extern int rkipc_log_level;

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#ifndef LOG_TAG
#define LOG_TAG "ipc-terminal"
#endif // LOG_TAG

#define LOG_INFO(format, ...)                                                                      \
    do {                                                                                           \
        if (rkipc_log_level >= LOG_LEVEL_INFO)                                                     \
            fprintf(stderr, GREEN_COLOR "[%s][%s]:" format RESET_COLOR, LOG_TAG, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#define LOG_WARN(format, ...)                                                                      \
    do {                                                                                           \
        if (rkipc_log_level >= LOG_LEVEL_WARN)                                                     \
            fprintf(stderr, YELLOW_COLOR "[%s][%s]:" format RESET_COLOR, LOG_TAG, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#define LOG_ERROR(format, ...)                                                                     \
    do {                                                                                           \
        if (rkipc_log_level >= LOG_LEVEL_ERROR)                                                    \
            fprintf(stderr, RED_COLOR "[%s][%s]:" format RESET_COLOR, LOG_TAG, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#define LOG_DEBUG(format, ...)                                                                     \
    do {                                                                                           \
        if (rkipc_log_level >= LOG_LEVEL_DEBUG)                                                    \
            fprintf(stderr, WHITE_COLOR "[%s][%s]:" format RESET_COLOR, LOG_TAG, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#endif // _LOG_H_
