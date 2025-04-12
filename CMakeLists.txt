cmake_minimum_required(VERSION 3.10)

# =============================================================================
# 基础配置：工程名、SDK路径、工具链设置
# =============================================================================

# 读取 SDK_PATH 环境变量
if(NOT DEFINED ENV{SDK_PATH})
message(FATAL_ERROR "SDK_PATH 环境变量未设置！请先执行: \nexport SDK_PATH=/path/to/your/luckfox-pico")
endif()

# 设置 SDK 路径和交叉编译工具链
set(SDK_PATH $ENV{SDK_PATH})
set(TOOLCHAIN_PATH ${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-rockchip830-linux-uclibcgnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/arm-rockchip830-linux-uclibcgnueabihf-g++)
set(CMAKE_FIND_ROOT_PATH ${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/sysroot)

# 工程名（不能放在读取 SDK_PATH 之前）
project(ipc-terminal)

# =============================================================================
# 查找依赖包
# =============================================================================
# OpenCV
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

# PThread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# =============================================================================
# 子目录构建（如 ONVIF Server、第三方库）
# =============================================================================
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ 3rdparty.out)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/code/onvif_server onvif_server.out)

# =============================================================================
# 链接目录
# =============================================================================
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/arm-rockchip830-linux-uclibcgnueabihf)

# =============================================================================
# 源码文件收集
# =============================================================================
set(APP_CXXFILE ${CMAKE_CURRENT_SOURCE_DIR}/code/app/main.cpp)
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/common)
set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/modules)
set(ONVIF_SERVER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code/onvif_server)
file(GLOB SRC_FILES 
    ${APP_CXXFILE}
    
    ${COMMON_DIR}/param/*.c

    ${MODULES_DIR}/Network/Network.cpp
    ${MODULES_DIR}/Network/ntp.c

    ${MODULES_DIR}/Control/Control.cpp

    ${MODULES_DIR}/Led/Led.cpp
    ${MODULES_DIR}/Led/gpio.c

    ${MODULES_DIR}/Pantilt/Pantilt.cpp
    ${MODULES_DIR}/Pantilt/pwm.c

    ${MODULES_DIR}/Display/Display.cpp
    ${MODULES_DIR}/Display/framebuffer.c
    
    ${MODULES_DIR}/Video/Video.cpp
    ${MODULES_DIR}/Video/luckfox_video.cpp
    ${MODULES_DIR}/Video/luckfox_rgn_draw.cpp
    ${MODULES_DIR}/Video/yolov5.cpp
    ${MODULES_DIR}/Video/postprocess.cpp
    ${MODULES_DIR}/Video/luckfox_rtsp.c
    ${MODULES_DIR}/Video/luckfox_osd.c
    ${MODULES_DIR}/Video/osd/*.c
)

# =============================================================================
# 生成可执行文件
# =============================================================================
add_executable(${PROJECT_NAME} ${SRC_FILES})

# =============================================================================
# 编译选项与定义
# =============================================================================
add_definitions(-DRV1106_1103)
add_compile_options(-g -Wall
                    -DISP_HW_V30 -DRKPLATFORM=ON -DARCH64=OFF
                    -DROCKIVA -DUAPI2
                    -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 
)

# =============================================================================
# 设置链接器参数
# =============================================================================
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed,-rpath-link,${CMAKE_CURRENT_SOURCE_DIR}/lib:/usr/lib")

set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/lib"
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    ${LIBRKNNRT}
    Threads::Threads

    rockiva
    sample_comm
    rockit
    rockchip_mpp
    rkaiq
    pthread
    rtsp
    freetype
    iconv

    onvif_server
)

# =============================================================================
# 头文件包含目录
# =============================================================================
target_include_directories(${PROJECT_NAME} PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/uAPI2
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/common
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/xcore
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/algos
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/iq_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/iq_parser_v2
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/smartIr
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rockchip
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rknn
    ${CMAKE_CURRENT_SOURCE_DIR}/include/freetype2
    
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rknpu2/include

    ${CMAKE_CURRENT_SOURCE_DIR}/code/common
    ${CMAKE_CURRENT_SOURCE_DIR}/code/common/log
    ${CMAKE_CURRENT_SOURCE_DIR}/code/common/param
    ${CMAKE_CURRENT_SOURCE_DIR}/code/common/signal
    
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Control
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Display
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Led
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Network
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Pantilt
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Video
    ${CMAKE_CURRENT_SOURCE_DIR}/code/modules/Video/osd

    ${CMAKE_CURRENT_SOURCE_DIR}/code/onvif_server/server
)

# =============================================================================
# 安装配置
# =============================================================================
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/model/ DESTINATION ${CMAKE_INSTALL_PREFIX}/model FILES_MATCHING PATTERN "*.*")

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/code/ipc-terminal.ini DESTINATION ${CMAKE_INSTALL_PREFIX})
