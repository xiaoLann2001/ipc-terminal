# Set the source files for the GPIO and LED dynamic libraries
file(GLOB GPIO_SRC "gpio/gpio.c")  # GPIO source file
file(GLOB LED_SRC "led/led.c")     # LED source file
file(GLOB FB_SRC "framebuffer/framebuffer.c")     # FrameBuffer source file

# Create dynamic library
add_library(gpio SHARED ${GPIO_SRC})  # Create shared library from GPIO source files
add_library(led SHARED ${LED_SRC})    # Create shared library from LED source files
add_library(fb SHARED ${FB_SRC})    # Create shared library from LED source files

target_link_libraries(led PUBLIC gpio)

# Set the output directory for shared libraries
set_target_properties(gpio
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)
set_target_properties(led 
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
    INSTALL_RPATH "$ORIGIN"  # set the runtime path for shared libraries
    BUILD_WITH_INSTALL_RPATH TRUE  # use the install rpath when building
)
set_target_properties(fb
    PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)

# Set include directories for the GPIO and LED libraries
target_include_directories(gpio PUBLIC ../include)
target_include_directories(led PUBLIC ../include)
target_include_directories(fb PUBLIC ../include)

# Optional: Specify any additional compile options for the GPIO and LED libraries
target_compile_options(gpio PRIVATE -g -Wall)
target_compile_options(led PRIVATE -g -Wall)
target_compile_options(fb PRIVATE -g -Wall)
