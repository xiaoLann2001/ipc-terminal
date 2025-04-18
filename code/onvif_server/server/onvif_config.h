// wsdd_config.h 或 onvif_config.h
#ifndef __ONVIF_CONFIG_H__
#define __ONVIF_CONFIG_H__

#define WS_DISCOVERY_PORT       3702
#define WEB_SERVICE_PORT        80
#define ONVIF_MULTICAST_IP      "239.255.255.250"

#define INFO_LENGTH 512
#define LARGE_INFO_LENGTH 1024
#define SMALL_INFO_LENGTH 512

// Media
#define MAX_MEDIA_PROFILE_NUM  2
#define MAIN_STREAM_PATH      "live/0"
#define SUB_STREAM_PATH       "live/1"

#endif // __ONVIF_CONFIG_H__
