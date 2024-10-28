cmake_minimum_required(VERSION 3.10)

# Set the SDK path and cross-compilers
set(SDK_PATH "/home/xiaolan/luckfox-pico")

set(CMAKE_C_COMPILER "${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc")
set(CMAKE_CXX_COMPILER "${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-g++")

# Project name
project(ipc-terminal)

# Add definition for specific drivers
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
add_definitions(-DRV1106_1103)

# Set the path to OpenCV
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

# Enable thread support (PThreads)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Add subdirectories for third-party and source code
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/arm-rockchip830-linux-uclibcgnueabihf)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ 3rdparty.out)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/code/src)

# Collect the main source file
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/src")
file(GLOB SRC_FILES "main.cpp" 
                    ${SRC_DIR}/Network/Network.cpp
                    ${SRC_DIR}/Control/Control.cpp

                    ${SRC_DIR}/led/led.c
                    ${SRC_DIR}/led/gpio.c

                    ${SRC_DIR}/Pantilt/Pantilt.cpp
                    ${SRC_DIR}/Pantilt/pwm.c

                    ${SRC_DIR}/Display/Display.cpp
                    ${SRC_DIR}/Display/framebuffer.c
                    
                    ${SRC_DIR}/Video/Video.cpp
                    ${SRC_DIR}/Video/luckfox_video.cpp
                    ${SRC_DIR}/Video/luckfox_rtsp.c
                    ${SRC_DIR}/Video/luckfox_rgn_draw.cpp
                    ${SRC_DIR}/Video/yolov5.cpp
                    ${SRC_DIR}/Video/postprocess.cpp
                    ${SRC_DIR}/Video/luckfox_osd.c

                    ${SRC_DIR}/Video/osd/*.c

                    ${SRC_DIR}/param/*.c
)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Set compile options for debugging and architecture-specific definitions
add_compile_options(-g -Wall
                    -DISP_HW_V30 -DRKPLATFORM=ON -DARCH64=OFF
                    -DROCKIVA -DUAPI2
                    -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 
)

# Set linker flags to handle runtime paths for shared libraries
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed,-rpath-link,${CMAKE_CURRENT_SOURCE_DIR}/lib:/usr/lib")

# Set the RPATH to $ORIGIN/lib for shared libraries
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/lib"
)

# Link the necessary libraries to the project
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    ${LIBRKNNRT}
    Threads::Threads
    # led
    # fb
    # video
    rockiva
    sample_comm
    rockit
    rockchip_mpp
    rkaiq
    pthread
    rtsp
    freetype
    iconv
)

# Set include directories for the project
target_include_directories(${PROJECT_NAME} PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/code/include
    ${CMAKE_CURRENT_SOURCE_DIR}/code/include/Video
    ${CMAKE_CURRENT_SOURCE_DIR}/code/include/Video/osd
    ${CMAKE_CURRENT_SOURCE_DIR}/code/include/log
    ${CMAKE_CURRENT_SOURCE_DIR}/code/include/param
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
)

# Set installation prefix for the project
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

# Install the binary into the install directory
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install model files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/model/ DESTINATION ${CMAKE_INSTALL_PREFIX}/model
        FILES_MATCHING PATTERN "*.*"    # Match all files in the model directory
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/code/ipc-terminal.ini DESTINATION ${CMAKE_INSTALL_PREFIX})

# # Install the shared libraries from code/lib into install/lib
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/lib/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
#         FILES_MATCHING PATTERN "*.so"   # Only match shared libraries
#         PATTERN "*.a" EXCLUDE           # Exclude static libraries if not needed
# )
