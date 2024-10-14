cmake_minimum_required(VERSION 3.10)

set(SDK_PATH "/home/xiaolan/luckfox-pico")

set(CMAKE_C_COMPILER "${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc")
set(CMAKE_CXX_COMPILER "${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-g++")

project(ipc-terminal)

# Link directories for libraries
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ 3rdparty.out)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/code/src) 

set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
add_definitions(-DRV1106_1103)

# OpenCV 4
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

# Thread support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find and add the source files
file(GLOB SRC_FILES "main.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Compile options for debugging and architecture-specific definitions
add_compile_options(-g -Wall
                    -DISP_HW_V30 -DRKPLATFORM=ON -DARCH64=OFF
                    -DROCKIVA -DUAPI2
                    -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 
                    )

# Linker flags to set the runtime path for shared libraries
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed,-rpath-link,${CMAKE_CURRENT_SOURCE_DIR}/lib:/usr/lib")

set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/lib"  # ORIGIN is the directory containing the executable
)

# Link the necessary libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
                    ${OpenCV_LIBS}
                    # ${LIBRKNNRT}
                    # Threads::Threads
                    # rockiva
                    # sample_comm
                    # rockit
                    # rockchip_mpp
                    # rkaiq
                    # pthread
                    # rtsp
                    led
                    fb
                    )

# Set include directories for the project
target_include_directories(${PROJECT_NAME} PUBLIC                        
                            ${OpenCV_INCLUDE_DIRS}
                            ${CMAKE_CURRENT_SOURCE_DIR}/code/include
                            # ${CMAKE_CURRENT_SOURCE_DIR}
                            # ${CMAKE_CURRENT_SOURCE_DIR}/utils
                            # ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rknpu2/include
                            # ${CMAKE_CURRENT_SOURCE_DIR}/common 
                            # ${CMAKE_CURRENT_SOURCE_DIR}/common/isp3.x   
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                            # ${CMAKE_CURRENT_SOURCE_DIR}/include/rknn
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/uAPI2
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/common
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/xcore
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/algos
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/iq_parser
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/iq_parser_v2
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/rkaiq/smartIr
                          )

# Set installation prefix
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

# Install the target binary into the install directory
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install the libraries from code/lib into install/lib
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/lib/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        FILES_MATCHING PATTERN "*.so"   # Match all shared libraries
        PATTERN "*.a" EXCLUDE)          # Optionally, you can exclude static libraries if you don't need them
