# 编译器选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DWITH_NO_C_LOCALE")

# 包含头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 源文件列表
set(SOURCES
    # soap生成的文件
    soapC.c
    soapServer.c
    soapClient.c
    stdsoap2.c

    # 服务器代码
    onvif_server.c
    onvif_server_interface.c

    # ONVIF服务实现
    service_device.c
    service_media.c
    service_ptz.c
    service_event.c

    # 工具函数
    utils.c
)

# 生成动态库（SHARED 表示生成 .so）
add_library(onvif_server SHARED ${SOURCES})

# 链接系统库
target_link_libraries(onvif_server m pthread)

# 设置安装路径
install(TARGETS onvif_server DESTINATION lib)
