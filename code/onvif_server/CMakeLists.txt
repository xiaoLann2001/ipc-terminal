# 编译器选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DWITH_NO_C_LOCALE")

# 常用功能库
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common)

# 包含头文件路径
include_directories(${COMMON_DIR})
include_directories(${COMMON_DIR}/log)
include_directories(${COMMON_DIR}/param)
include_directories(${COMMON_DIR}/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/soap)

# 源文件列表
set(SOURCES
    # soap生成的文件
    ${CMAKE_CURRENT_SOURCE_DIR}/soap/soapC.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soap/soapServer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soap/soapClient.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soap/stdsoap2.c

    # 服务器代码
    ${CMAKE_CURRENT_SOURCE_DIR}/server/onvif_server.c
    
    # ONVIF服务实现
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/onvif_server_interface.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/service_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/service_media.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/service_ptz.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/service_event.c

    # 工具函数
    ${COMMON_DIR}/utils/utils.c
)

# 生成动态库（SHARED 表示生成 .so）
add_library(onvif_server SHARED ${SOURCES})

# 链接系统库
target_link_libraries(onvif_server m pthread)

# 设置安装路径
install(TARGETS onvif_server DESTINATION lib)
