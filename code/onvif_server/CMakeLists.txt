# 编译器选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DWITH_NO_C_LOCALE")

# 包含头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/soap)

# 源文件列表
set(SOURCES
    # soap生成的文件
    soap/soapC.c
    soap/soapServer.c
    soap/soapClient.c
    soap/stdsoap2.c

    # 服务器代码
    server/onvif_server.c
    
    # ONVIF服务实现
    interface/onvif_server_interface.c
    interface/service_device.c
    interface/service_media.c
    interface/service_ptz.c
    interface/service_event.c

    # 工具函数
    common/utils.c
)

# 生成动态库（SHARED 表示生成 .so）
add_library(onvif_server SHARED ${SOURCES})

# 链接系统库
target_link_libraries(onvif_server m pthread)

# 设置安装路径
install(TARGETS onvif_server DESTINATION lib)
